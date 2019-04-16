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

#ifndef SPEECH_BT_NODES_H
#define SPEECH_BT_NODES_H

#include <behaviortree_cpp/behavior_tree.h>

class CommandIs : public BT::ConditionNode
{
public:
    CommandIs(const std::string& name, const BT::NodeConfiguration& config);


    BT::NodeStatus tick() override;


    // A node having ports MUST implement this STATIC method
    static BT::PortsList providedPorts();

};


class SayText : public BT::SyncActionNode
{
public:
    SayText(const std::string& name, const BT::NodeConfiguration& config);


    BT::NodeStatus tick() override;


    // A node having ports MUST implement this STATIC method
    static BT::PortsList providedPorts();

};




class TakeAt : public BT::SyncActionNode
{
public:
    TakeAt(const std::string& name, const BT::NodeConfiguration& config);


    BT::NodeStatus tick() override;


    // A node having ports MUST implement this STATIC method
    static BT::PortsList providedPorts();

};


class ShowObject : public BT::SyncActionNode
{
public:
    ShowObject(const std::string& name, const BT::NodeConfiguration& config);


    BT::NodeStatus tick() override;


    // A node having ports MUST implement this STATIC method
    static BT::PortsList providedPorts();

};





#endif // SPEECH_BT_NODES_H
