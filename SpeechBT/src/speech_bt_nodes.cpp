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


#include <speech_bt_nodes.h>
#include <behaviortree_cpp/behavior_tree.h>


/*CommandIs*/

CommandIs::CommandIs(const std::string& name, const BT::NodeConfiguration& config)
    : BT::ConditionNode(name, config)
{
}

BT::NodeStatus CommandIs::tick()
{


    auto target_msg = getInput<std::string>("target_command");
    auto current_msg = getInput<std::string>("current_command");

    if (target_msg.value().compare(current_msg.value())!=0)
    {
        std::cout << "Target Command: " << target_msg.value() << std::endl;
        std::cout << "Current Command: " << current_msg.value() << std::endl;

        return BT::NodeStatus::FAILURE;
    }
    else
    {
        std::cout << "Target Command: " << target_msg.value() << std::endl;
        std::cout << "Current Command: " << current_msg.value() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
}

BT::PortsList CommandIs::providedPorts()
{
    return { BT::InputPort<std::string>("current_command"), BT::InputPort<std::string>("target_command") };
}




/*SayText*/


SayText::SayText(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
{
}

BT::NodeStatus SayText::tick()
{
    auto msg = getInput<std::string>("text");
    if (!msg)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg.error() );
    }

    std::cout << "Robot says: " << msg.value() << std::endl;
    return BT::NodeStatus::SUCCESS;

}

// It is mandatory to define this static method.
BT::PortsList SayText::providedPorts()
{
    return{ BT::InputPort<std::string>("text") };
}

/*TakeAt*/

TakeAt::TakeAt(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
{
}

// You must override the virtual function tick()
BT::NodeStatus TakeAt::tick()
{
    auto msg_object = getInput<std::string>("object");
    auto msg_at = getInput<std::string>("at");
    if (!msg_object)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg_object.error() );
    }

    if (!msg_at)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg_at.error() );
    }

    std::cout << "Take at failed " << std::endl;

    setOutput("error_message_port", "TakeError" );

    return BT::NodeStatus::FAILURE;

}

// It is mandatory to define this static method.
BT::PortsList TakeAt::providedPorts()
{
    return{ BT::InputPort<std::string>("object"), BT::InputPort<std::string>("at"), BT::OutputPort<std::string>("error_message_port") };
}



/*ShowObject*/



ShowObject::ShowObject(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
{
}

// You must override the virtual function tick()
BT::NodeStatus ShowObject::tick()
{
    auto msg_object = getInput<std::string>("object");
    if (!msg_object)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg_object.error() );
    }

    std::cout << "Take at failed " << std::endl;

    setOutput("error_message_port", "ShowError" );
    return BT::NodeStatus::FAILURE;

}

// It is mandatory to define this static method.
BT::PortsList ShowObject::providedPorts()
{
    return{ BT::InputPort<std::string>("object"),  BT::OutputPort<std::string>("error_message_port") };
}



