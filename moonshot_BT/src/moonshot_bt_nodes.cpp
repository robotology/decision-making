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


#include <moonshot_bt_nodes.h>
#include <behaviortree_cpp/behavior_tree.h>
#include <yarp/os/Network.h>
#include <yarp/os/RpcServer.h>
#include <yarp/dev/PolyDriver.h> // for navigation
#include <yarp/dev/INavigation2D.h> // for navigation
#include <yarp/os/LogStream.h> // for yError()
/*CommandIs*/

CommandIs::CommandIs(const std::string& name, const BT::NodeConfiguration& config)
    : BT::ConditionNode(name, config)
{
}

BT::NodeStatus CommandIs::tick()
{


    auto target_msg = getInput<std::string>("target_command");
    auto current_msg = getInput<std::string>("current_command");

    if (current_msg.value().compare(target_msg.value())!=0)
    {
        std::cout << "Current Command: " << current_msg.value() << std::endl;
        std::cout << "Target Command: " << target_msg.value() << std::endl;
        std::cout << "Condition Failing " << target_msg.value() << std::endl;

        return BT::NodeStatus::FAILURE;
    }
    else
    {
        std::cout << "Current Command: " << current_msg.value() << std::endl;
        std::cout << "Target Command: " << target_msg.value() << std::endl;
        std::cout << "Condition Succeeding " << target_msg.value() << std::endl;

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
    Network yarp;
    auto msg = getInput<std::string>("text");
    std::string port_out_name ="/moonshot/BTSpeech/" + name +"/synthesize:o";
    std::string port_in_name ="/moonshot/synthesize:i";

    text_to_say_port_.open(port_out_name);
    yarp.connect(port_out_name, port_in_name);



}

BT::NodeStatus SayText::tick()
{

    Network yarp;


    auto msg = getInput<std::string>("text");
    if (!msg)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg.error() );
    }

    yarp::os::Bottle cmd;
    yarp::os::Bottle response;

    cmd.addString(msg.value());

    text_to_say_port_.write(cmd);

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



    std::this_thread::sleep_for (std::chrono::seconds(5)); // Simulate action

    bool action_ok = true;

    if (action_ok)
    {
        std::cout << "Taking "<< msg_object.value() << std::endl;

        return BT::NodeStatus::SUCCESS;
    }
    else
    {
        setOutput("error_message_port", "TakeError" );
        return BT::NodeStatus::FAILURE;
    }

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


    bool action_ok = true;

    if (action_ok)
    {
        return BT::NodeStatus::SUCCESS;
    }
    else
    {
        setOutput("error_message_port", "ShowError" );
        return BT::NodeStatus::FAILURE;
    }


}

// It is mandatory to define this static method.
BT::PortsList ShowObject::providedPorts()
{
    return{ BT::InputPort<std::string>("object"),  BT::OutputPort<std::string>("error_message_port") };
}





/*GoTo*/

GoTo::GoTo(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
{

    Network yarp;

    //opens a navigation2DClient device to control the robot
    navigClientCfg_.put("device", "navigation2DClient");
    navigClientCfg_.put("local", "/robotPathPlannerExample");
    navigClientCfg_.put("navigation_server", "/navigationServer");
    navigClientCfg_.put("map_locations_server", "/mapServer");
    navigClientCfg_.put("localization_server", "/localizationServer");
    bool ok = ddNavClient_.open(navigClientCfg_);
    if (!ok)
    {
        yError() << "Unable to open navigation2DClient device driver";
    }

    ok = ddNavClient_.view(iNav_);
    if (!ok)
    {
        yError() << "Unable to open INavigation2D interface";
    }




}

// You must override the virtual function tick()
BT::NodeStatus GoTo::tick()
{
    Network yarp;

    auto msg_object = getInput<std::string>("target");
    if (!msg_object)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg_object.error() );
    }
    yarp::dev::NavigationStatusEnum status;

    yarp::dev::Map2DLocation pos,destination;



    destination.x = 10;
    destination.y = 3;
    destination.theta = 0;
    destination.map_id = "sanquirico";
    //Gets the current navigation status

    iNav_->stopNavigation();
    iNav_->gotoTargetByAbsoluteLocation(destination);

    bool action_ok = false;


    while (true)
    {
        iNav_->getNavigationStatus(status);
         if (status == yarp::dev::NavigationStatusEnum::navigation_status_goal_reached)
         {
             yInfo() << "Robot has reached the destination";

             action_ok = true;
             break;
         }
         else if (status == yarp::dev::NavigationStatusEnum::navigation_status_error)
         {
             yError() << "Robot cannot reach the destination";

             action_ok = false;
             break;
         }

         else if (status == yarp::dev::NavigationStatusEnum::navigation_status_moving)
         {
             yInfo() << "The Robot is Moving";
         }
         else
         {
             yError() << "Cannot reach the destination";
         }

         std::this_thread::sleep_for (std::chrono::seconds(1));
    }




    if (action_ok)
    {
        return BT::NodeStatus::SUCCESS;
    }
    else
    {
        setOutput("error_message_port", "GotoError" );
        return BT::NodeStatus::FAILURE;
    }

}

// It is mandatory to define this static method.
BT::PortsList GoTo::providedPorts()
{
    return{ BT::InputPort<std::string>("target"),  BT::OutputPort<std::string>("error_message_port") };
}




/*ListenCommand*/



ListenCommand::ListenCommand(const std::string& name, const BT::NodeConfiguration& config)
    : BT::SyncActionNode(name, config)
{
    Network yarp;

    bool ok = port_.open("/moonshot/BTSpeech/recognise:rpc:i");

//    if (ok)
//    {

//    }
//    else
//    {

//    }

}

// You must override the virtual function tick()
BT::NodeStatus ListenCommand::tick()
{
    Network yarp;

    yarp::os::Bottle cmd;
    yarp::os::Bottle response;
    Bottle *input=port_.read(false);

if (input!=NULL) {
    std::string command = input->get(0).asString();

    if(command.compare("take") == 0)
    {

        setOutput("command_listened", "take" );

        std::string object = input->get(1).asString();
        std::string location = input->get(2).asString();

        setOutput("object_listened", object);

        setOutput("location_listened", location );

    }
    else if(command.compare("show") == 0)
    {
        setOutput("command_listened", "show" );

        std::string location = input->get(1).asString();
        setOutput("location_listened", location );

    }
    else if(command.compare("goto") == 0)
    {
        setOutput("command_listened", "goto" );

        std::string location = input->get(1).asString();
        setOutput("location_listened", location );

    }
    else
    {
        setOutput("error_message_port", "command not recognized" );
    }
}

//    auto msg_object = getInput<std::string>("object");
//    if (!msg_object)
//    {
//        throw BT::RuntimeError( "missing required input [message]: ", msg_object.error() );
//    }

//    std::cout << "Take at failed " << std::endl;

//    setOutput("error_message_port", "ShowError" );
    return BT::NodeStatus::SUCCESS;

}


// It is mandatory to define this static method.
BT::PortsList ListenCommand::providedPorts()
{
    return{ BT::OutputPort<std::string>("command_listened"), BT::OutputPort<std::string>("object_listened"), BT::OutputPort<std::string>("location_listened"),  BT::OutputPort<std::string>("error_message_port") };
}



