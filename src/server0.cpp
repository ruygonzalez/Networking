/**
@file
@author Ben Yuan
@date 2013
@copyright 2-clause BSD; see License section

@brief
A simple echo server to validate the network wrapper.

@section License

Copyright (c) 2012-2013 California Institute of Technology.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the California Institute of Technology.

*/

#include "server0.hpp"

using namespace CS2Net;

int main(int argc, char ** argv)
{
    //Add a connection
    REQUIRE(argc == 2, "usage: %s port", argv[0]);
	CS2Net::Socket listener;
    CS2Net::Socket * incoming_conn = nullptr;
    
    uint16_t port = atoi(argv[1]);
    int err = listener.Bind(port, 1);
    REQUIRE(err == 0, "Failed to bind!");
    
    // Receive a message
    incoming_conn = listener.Accept();
    REQUIRE(incoming_conn != nullptr, "Failed to accept!");
    std::string * incoming = incoming_conn->Recv(1024, false);
    if(incoming == NULL)
    {
        // bad stuff happened
        ERROR("recv error: %s", strerror(errno));
    }
    else
    {
        // we got some data yay
        // Now return it
        std::string to_send(*incoming);
        int ret = incoming_conn->Send(&to_send);
        if(ret < 0)
        {
        // bad stuff happened
            if(ret == -1)
            {
                ERROR("send error: %s", strerror(errno));
            }
            else
            {
                ERROR("this error should never occur");
            }
        }
     }
    

    delete incoming_conn;
    /* 
     *
     * Accept a connection, recv a message, send it back, close the connection.
     * Rinse and repeat forever.
     */

    return 0;
}

