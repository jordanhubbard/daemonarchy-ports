#include <gpg-error.h>

#include <errno.h>
#include <stdio.h>

int
main(void)
{
	gpg_error_t error;
	const char *message;

	error = gpg_error_from_errno(EACCES);
	message = gpg_strerror(error);
	if (gpg_err_code(error) != GPG_ERR_EACCES ||
	    gpg_err_code_to_errno(gpg_err_code(error)) != EACCES ||
	    message == NULL || message[0] == '\0')
		return (1);
	puts("static pkg-config consumer: PASS");
	return (0);
}
