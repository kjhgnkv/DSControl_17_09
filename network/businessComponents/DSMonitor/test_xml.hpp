#pragma once

#include <string>

#if _WIN32

std::string xml_deploy = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                         "<root>\n"
                         "    <ApplicationPath>..\\..\\DSHost1</ApplicationPath>\n"
                         "    <CommandType>\n"
                         "\tDeploy\n"
                         "</CommandType>\n"
                         "    <ApplicationName>\n"
                         "\tDSHost1\n"
                         "</ApplicationName>\n"
                                "    <Components>\n"
                                "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                                "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                                "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"192.168.100.21:60000\"/>\n"
                                "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                                "            </Object>\n"
                                "            <Object name=\"object2\" ds_content=\"object2-content\">\n"
                                "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" of=\"192.168.100.21:60001\"/>\n"
                                "                <Send name=\"object2.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                                "            </Object>\n"
                                "        </Component>\n"
                                "    </Components>\n"
                                "</root>";

std::string xml_deploy_2 = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                           "<root>\n"
                           "    <ApplicationPath>..\\..\\DSHost2</ApplicationPath>\n"
                           "    <CommandType>\n"
                                  "\tDeploy\n"
                                  "</CommandType>\n"
                                  "    <ApplicationName>\n"
                                  "\tDSHost2\n"
                                  "</ApplicationName>\n"
                                  "    <Components>\n"
                                  "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                                  "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                                  "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"127.0.0.1:60000\"/>\n"
                                  "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                                  "            </Object>\n"
                                  "            <Object name=\"Obj2\" ds_content=\"object2-content\">\n"
                                  "                <Comm name=\"tcp-l\" id=\"1\" method=\"udp\" of=\"127.0.0.1:60000\"/>\n"
                                  "                <Recv name=\"Obj2.receiver2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                                  "            </Object>\n"
                                  "        </Component>\n"
                                  "    </Components>\n"
                                  "</root>";

std::string xml_deploy_2_crash = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                 "<root>\n"
                                 "    <ApplicationPath>..\\..\\DSHost2</ApplicationPath>\n"
                                 "    <CommandType>\n"
                                        "\tDeploy\n"
                                        "</CommandType>\n"
                                        "    <ApplicationName>\n"
                                        "\tDSHost2\n"
                                        "</ApplicationName>\n"
                                        "    <Components>\n"
                                        "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                                        "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                                        "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"127.0.0.1:60000\"/>\n"
                                        "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n";

std::string xml_update = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                         "<root>\n"
                         "    <ApplicationPath>..\\..\\DSHost1</ApplicationPath>\n"
                         "    <CommandType>\n"
                                      "\tUpdate\n"
                                      "</CommandType>\n"
                                      "    <ApplicationName>\n\tDSHost1\n"
                                      "</ApplicationName>\n"
                                      "    <Components>\n"
                                      "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                                      "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                                      "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"127.0.0.1:61000\"/>\n"
                                      "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"all\"/>\n"
                                      "            </Object>\n"
                                      "            <Object name=\"Obj2\" ds_content=\"object2-content\">\n"
                                      "                <Comm name=\"tcp-l\" id=\"1\" method=\"udp\" if=\"127.0.0.1:61000\"/>\n"
                                      "                <Recv name=\"Obj2.receiver2\" comm=\"tcp-l\" protocol=\"raw\" id=\"all\"/>\n"
                                      "            </Object>\n"
                                      "        </Component>\n"
                                      "    </Components>\n"
                                      "</root>";

std::string xml_stop = "<root>\n"
                       "    <ApplicationPath>..\\..\\DSHost1</ApplicationPath>\n"
                       "    <CommandType>\n"
                                    "\tStop\n"
                                    "</CommandType>\n"
                                    "    <ApplicationName>\n"
                                    "\tDSHost1\n"
                                    "</ApplicationName>\n"
                                    "</root>";

std::string xml_stop_crash = "<root>\n"
                             "    <ApplicationPath>..\\..\\DSHost1</ApplicationPath>\n"
                             "    <CommandType>\n"
                                          "\tStop\n"
                                          "</CommandType>\n"
                                          "    <ApplicationName>\n"
                                          "\tDSHost1\n";

std::string xml_stop_2 = "<root>\n"
                         "    <ApplicationPath>..\\..\\DSHost2</ApplicationPath>\n"
                         "    <CommandType>\n"
                                      "\tStop\n"
                                      "</CommandType>\n"
                                      "    <ApplicationName>\n"
                                      "\tDSHost2\n"
                                      "</ApplicationName>\n"
                                      "</root>";
#elif __linux

std::string xml_deploy = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                         "<root>\n"
                         "    <ApplicationPath>../../DSHost1</ApplicationPath>\n"
                         "    <CommandType>\n"
                         "\tDeploy\n"
                         "</CommandType>\n"
                         "    <ApplicationName>\n"
                         "\tDSHost1\n"
                         "</ApplicationName>\n"
                         "    <Components>\n"
                         "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                         "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                         "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"192.168.100.21:60000\"/>\n"
                         "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                         "            </Object>\n"
                         "            <Object name=\"object2\" ds_content=\"object2-content\">\n"
                         "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" of=\"192.168.100.21:60001\"/>\n"
                         "                <Send name=\"object2.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                         "            </Object>\n"
                         "        </Component>\n"
                         "    </Components>\n"
                         "</root>";

std::string xml_deploy_2 = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                           "<root>\n"
                           "    <ApplicationPath>../../DSHost2</ApplicationPath>\n"
                           "    <CommandType>\n"
                           "\tDeploy\n"
                           "</CommandType>\n"
                           "    <ApplicationName>\n"
                           "\tDSHost2\n"
                           "</ApplicationName>\n"
                           "    <Components>\n"
                           "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                           "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                           "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"127.0.0.1:60000\"/>\n"
                           "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                           "            </Object>\n"
                           "            <Object name=\"Obj2\" ds_content=\"object2-content\">\n"
                           "                <Comm name=\"tcp-l\" id=\"1\" method=\"udp\" of=\"127.0.0.1:60000\"/>\n"
                           "                <Recv name=\"Obj2.receiver2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n"
                           "            </Object>\n"
                           "        </Component>\n"
                           "    </Components>\n"
                           "</root>";

std::string xml_deploy_2_crash = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                 "<root>\n"
                                 "    <ApplicationPath>../../DSHost2</ApplicationPath>\n"
                                 "    <CommandType>\n"
                                 "\tDeploy\n"
                                 "</CommandType>\n"
                                 "    <ApplicationName>\n"
                                 "\tDSHost2\n"
                                 "</ApplicationName>\n"
                                 "    <Components>\n"
                                 "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                                 "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                                 "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"127.0.0.1:60000\"/>\n"
                                 "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"1\"/>\n";

std::string xml_update = "<?Parser version=\"1.0\" encoding=\"UTF-8\"?>\n"
                         "<root>\n"
                         "    <ApplicationPath>../../DSHost1</ApplicationPath>\n"
                         "    <CommandType>\n"
                         "\tUpdate\n"
                         "</CommandType>\n"
                         "    <ApplicationName>\n\tDSHost1\n"
                         "</ApplicationName>\n"
                         "    <Components>\n"
                         "        <Component name=\"ds.host.samples.echo\" content_id=\"echo_msg\">\n"
                         "            <Object name=\"object1\" ds_content=\"object1-content\">\n"
                         "                <Comm name=\"tcp-l\" id=\"2\" method=\"udp\" if=\"127.0.0.1:61000\"/>\n"
                         "                <Send name=\"object1.sender2\" comm=\"tcp-l\" protocol=\"raw\" id=\"all\"/>\n"
                         "            </Object>\n"
                         "            <Object name=\"Obj2\" ds_content=\"object2-content\">\n"
                         "                <Comm name=\"tcp-l\" id=\"1\" method=\"udp\" if=\"127.0.0.1:61000\"/>\n"
                         "                <Recv name=\"Obj2.receiver2\" comm=\"tcp-l\" protocol=\"raw\" id=\"all\"/>\n"
                         "            </Object>\n"
                         "        </Component>\n"
                         "    </Components>\n"
                         "</root>";

std::string xml_stop = "<root>\n"
                       "    <ApplicationPath>../../DSHost1</ApplicationPath>\n"
                       "    <CommandType>\n"
                       "\tStop\n"
                       "</CommandType>\n"
                       "    <ApplicationName>\n"
                       "\tDSHost1\n"
                       "</ApplicationName>\n"
                       "</root>";

std::string xml_stop_crash = "<root>\n"
                             "    <ApplicationPath>../../DSHost1</ApplicationPath>\n"
                             "    <CommandType>\n"
                             "\tStop\n"
                             "</CommandType>\n"
                             "    <ApplicationName>\n"
                             "\tDSHost1\n";

std::string xml_stop_2 = "<root>\n"
                         "    <ApplicationPath>../../DSHost2</ApplicationPath>\n"
                         "    <CommandType>\n"
                         "\tStop\n"
                         "</CommandType>\n"
                         "    <ApplicationName>\n"
                         "\tDSHost2\n"
                         "</ApplicationName>\n"
                         "</root>";

#endif