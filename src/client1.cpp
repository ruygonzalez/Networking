/**
@file
@author Ben Yuan
@date 2013
@copyright 2-clause BSD; see License section

@brief
A more complex echo client to validate the network wrapper.

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

#include "client1.hpp"

using namespace CS2Net;

int main(int argc, char ** argv)
{

    REQUIRE(argc == 3, "usage: %s hostname port", argv[0]);
    CS2Net::Socket sock;
    std::string hostname(argv[1]);
    uint16_t port = atoi(argv[2]);
    // Connect
    int ret = sock.Connect(&hostname, port);
    if(ret < 0)
    {
    // something terrifying happened x_X
        if(ret == -1)
        {
            ERROR("connect error: %s", strerror(errno));
        }
        else if(ret == -3)
        {
            ERROR("connect error: %s", gai_strerror(errno));
        }
        else
        {
        ERROR("this error should never occur");
        }
    }
    else
    {
    // we connected yay
    // Create poll vector
        std::vector<CS2Net::PollFD> poll_vec(1);    
        poll_vec[0].sock = &sock;
        poll_vec[0].SetRead(true);
    // Initialize some time variables
    time_t past = time(nullptr);  
    time_t current;
        // Infinite loop to keep reading in data/sending messages
        while(true){
            // Sets current to current time
            time(&current);
            // get time elapsed from last check
            double interval = difftime(current, past);
            if(interval >= 1.0){
                // Attempt to send message
                std::string to_send("Tom Brady is the GOAT, also Hi");
                int ret = sock.Send(&to_send);
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
                else
                {
                // we sent some data yay
                // Now set past time to current time (or current when we last checked)
                past = current;
                }
            }
            // now do the poll (10 ms timeout)
            int poll_err = CS2Net::Poll(&poll_vec, 10);
            REQUIRE(poll_err >= 0, "error on poll!?");

            // is there a hangup or error?
            if(poll_vec[0].HasHangup() || poll_vec[0].HasError())
            {
                // o noes there's a hangup and/or error
                std::cout<< "Error: Hangup or poll has error" <<std::endl;
                // terminate program
                break;
            }
            // did we get anything to read?
            if(poll_vec[0].CanRead())
            {
            // yay the first socket is readable
            // Now read
                std::string * incoming = sock.Recv(1024, false);
                if(incoming == NULL)
                {
                // bad stuff happened
                    ERROR("recv error: %s", strerror(errno));
                }
                else
                {
                // we got some data yay
                // Print message
                    std::cout<< *incoming << std::endl;
                }
            }
        }
    }

    /*
    *
    * This should be a client that connects to a remote server,
    * sends a message of your choice no more than once per second,
    * and echoes any messages it receives in the meantime
    * without having to block on recv.
    *
    **/
    // Disconnect (unreachable code since infinite loop/goes on forever?)
	int ret2 = sock.Disconnect();
    if(ret2 < 0)
    {
    // bad stuff happened
        if(ret2 == -1)
        {
            ERROR("disconnect error: %s", strerror(errno));
        }
        else
        {
            ERROR("this error should never occur");
        }
    }

    return 0;


}
