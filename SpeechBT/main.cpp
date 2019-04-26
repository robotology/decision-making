/*
 * Copyright (C) 2019 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Michele Colledanchise
 * email:  michele.colledanchise@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#include <behaviortree_cpp/behavior_tree.h>

#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>
#include <behaviortree_cpp/loggers/bt_minitrace_logger.h>
#include <behaviortree_cpp/loggers/bt_file_logger.h>
#include <behaviortree_cpp/loggers/bt_zmq_publisher.h>
#include <iostream>
#include <speech_bt_nodes.h>


using namespace BT;




int main()
{

    BehaviorTreeFactory factory;



    factory.registerNodeType<SayText>("SayText");
    factory.registerNodeType<CommandIs>("CommandIs");
    factory.registerNodeType<TakeAt>("TakeAt");
    factory.registerNodeType<ShowObject>("ShowObject");
    factory.registerNodeType<ListenCommand>("ListenCommand");




    // clang-format off
    static const char* xml_text = R"(

            <root main_tree_to_execute = "MainTree">
            <BehaviorTree ID="MainTree">
            <Sequence name = "root">
            <ListenCommand   name= "Listen" object_listened="{object}"  location_listened="{location}" command_listened="{current_command}" error_message_port="{message_listen}"/>
            <Fallback name="root">
            <CommandIs   name= "Command is Stop"  target_command="stop" current_command="{current_command}" />
            <Sequence name="seq take">
                <CommandIs   name= "Command is Take" target_command="take" current_command="{current_command}" />
            <Fallback name="fal take">
                <TakeAt   object="{object}" at="{location}" error_message_port="{message_take}"/>
                <SayText     text="{message_take}" />
            </Fallback>
            </Sequence>
            <Sequence name="seq show">
                <CommandIs   name= "Command is Show"  target_command="show" current_command="{current_command}" />
            <Fallback name="fal show">
                <ShowObject   object="{object}" error_message_port="{message_show}"/>
                <SayText     text="{message_show}" />
            </Fallback>
            </Sequence>
            </Fallback>
            </Sequence>
            </BehaviorTree>

            </root>
            )";
            // clang-format on

    auto tree = factory.createTreeFromText(xml_text);
    tree.rootBlackboard()->set("error_messages", "" );
    tree.rootBlackboard()->set("current_command", "stop" );
    tree.rootBlackboard()->set("object", "table" );
    tree.rootBlackboard()->set("location", "" );

//    <SetBlackboard   output_key="current_command" value="stop" />



    StdCoutLogger logger_cout(tree);
    MinitraceLogger logger_minitrace(tree, "bt_trace.json");
    FileLogger logger_file(tree, "bt_trace.fbl");
    PublisherZMQ publisher_zmq(tree);

    while(1)
    {
        tree.root_node->executeTick();
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }


    return 0;
}
