/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller ( mueller@kde.org )
 * Copyright (C) 2003-2020 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Andrew Wellington (proton@wiretapped.net)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include <wtf/Platform.h>
#include <wtf/text/StringImpl.h>
#include <cstdio>
namespace WTF {
Ref<StringImpl> StringImpl::adopt(StringBuffer<LChar>&& buffer)
{
    unsigned length = buffer.length();
    if (!length)
        return *empty();
    return adoptRef(*new StringImpl(buffer.release(), length));
}

Ref<StringImpl> StringImpl::adopt(StringBuffer<UChar>&& buffer)
{
    unsigned length = buffer.length();
    if (!length)
        return *empty();
    return adoptRef(*new StringImpl(buffer.release(), length));
}

}
template<typename C> int bufferCase(unsigned length) {
    WTF::StringBuffer<C> data(length);
    for (unsigned i=0;i<length;++i) data[i]=static_cast<C>(i%127+1);
    auto string=WTF::StringImpl::adopt(WTFMove(data));
    if (string->length()!=length || string->is8Bit()!=(sizeof(C)==1)) return 10;
    for (unsigned i=0;i<length;++i) if (string->at(i)!=static_cast<C>(i%127+1)) return 11;
    return 0;
}
template<typename C> int vectorCase(unsigned length) {
    WTF::Vector<C,0,WTF::CrashOnOverflow,16,WTF::StringImplMalloc> data;
    for (unsigned i=0;i<length;++i) data.append(static_cast<C>(i%127+1));
    auto string=WTF::StringImpl::adopt(WTFMove(data));
    if (string->length()!=length || string->is8Bit()!=(sizeof(C)==1)) return 12;
    for (unsigned i=0;i<length;++i) if (string->at(i)!=static_cast<C>(i%127+1)) return 13;
    return 0;
}
int main() {
    std::printf("libcpp_version=%d hardening_mode=%d\n",_LIBCPP_VERSION,_LIBCPP_HARDENING_MODE);
    std::fflush(stdout);
    for (unsigned n : {1U,16U,127U,4096U}) {
        if (auto r=bufferCase<LChar>(n)) return r;
        if (auto r=bufferCase<UChar>(n)) return r;
        if (auto r=vectorCase<LChar>(n)) return r;
        if (auto r=vectorCase<UChar>(n)) return r;
    }
    std::puts("PASS 16 cases: StringBuffer copy and StringImplMalloc Vector transfer, 8/16bit, 4 lengths");
}
