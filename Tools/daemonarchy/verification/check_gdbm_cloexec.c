/* Authored native regression controls; original upstream tests stay unchanged. */
#include <gdbm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void require(int ok, int line) {
  if(!ok) { fprintf(stderr,"control setup failed at %d: %s\n",line,gdbm_strerror(gdbm_errno)); exit(2); }
}
#define REQUIRE(x) require((x),__LINE__)
static int exec_closed(const char *self,int fd) {
  char number[32]; REQUIRE(snprintf(number,sizeof(number),"%d",fd)>0);
  pid_t child=fork(); REQUIRE(child>=0);
  if(child==0) { execl(self,self,"inspect-exec",number,(char *)NULL); _exit(127); }
  int status; REQUIRE(waitpid(child,&status,0)==child); REQUIRE(WIFEXITED(status));
  int result=WEXITSTATUS(status); REQUIRE(result==0 || result==1); return result;
}
int main(int argc,char **argv) {
  if(argc==3 && strcmp(argv[1],"inspect-exec")==0) {
    errno=0; int flags=fcntl(atoi(argv[2]),F_GETFD);
    if(flags==-1 && errno==EBADF)return 1;
    return flags>=0?0:2;
  }
  REQUIRE(argc==1); REQUIRE(gdbm_version_number[0]==1 && gdbm_version_number[1]==26);
  unsigned failed=0; printf("{\"uid\":%lu,\"cases\":[",(unsigned long)getuid());
  for(int supplied=0;supplied<2;++supplied)for(int cloexec=0;cloexec<2;++cloexec) {
    char path[64]; REQUIRE(snprintf(path,sizeof(path),"control-%d-%d.db",supplied,cloexec)>0);
    int flags=GDBM_NEWDB|(cloexec?GDBM_CLOEXEC:0); GDBM_FILE db;
    if(supplied) {
      int fd=open(path,O_RDWR|O_CREAT|O_EXCL,0600); REQUIRE(fd>=0);
      db=gdbm_fd_open(fd,path,0,flags|GDBM_CLOERROR,NULL);
    } else db=gdbm_open(path,0,flags,0600,NULL);
    REQUIRE(db!=NULL);
    int initial=(fcntl(gdbm_fdesc(db),F_GETFD)&FD_CLOEXEC)!=0;
    int initial_exec=exec_closed(argv[0],gdbm_fdesc(db));
    char key[]="binary\0key",value[]="payload\0value";
    datum k={key,sizeof(key)},v={value,sizeof(value)};
    REQUIRE(gdbm_store(db,k,v,GDBM_INSERT)==0); REQUIRE(gdbm_reorganize(db)==0);
    int reorganized=(fcntl(gdbm_fdesc(db),F_GETFD)&FD_CLOEXEC)!=0;
    int reorganized_exec=exec_closed(argv[0],gdbm_fdesc(db));
    datum fetched=gdbm_fetch(db,k); REQUIRE(fetched.dsize==(int)sizeof(value) && memcmp(fetched.dptr,value,sizeof(value))==0); free(fetched.dptr);
    REQUIRE(gdbm_close(db)==0);
    int ok=initial==cloexec && initial_exec==cloexec && reorganized==cloexec && reorganized_exec==cloexec;
    failed+=!ok;
    printf("%s{\"supplied_fd\":%d,\"requested_cloexec\":%d,\"initial_cloexec\":%d,\"initial_exec_closed\":%d,\"reorganized_cloexec\":%d,\"reorganized_exec_closed\":%d,\"passed\":%s}",supplied||cloexec?",":"",supplied,cloexec,initial,initial_exec,reorganized,reorganized_exec,ok?"true":"false");
  }
  printf("],\"passed\":%u,\"failed\":%u}\n",4-failed,failed); return failed?1:0;
}
