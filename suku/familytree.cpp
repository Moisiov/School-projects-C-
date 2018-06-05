/*
#############################################################################
# TIE-02200 Ohjelmoinnin peruskurssi, K2018                                 #
# Maker: Valto Moisio, 268644                                               #
# Project3: Suku on pahin                                                   #
# File: familytree.cpp                                                      #
# Description: Familytree-class source file, including programs             #
# functionality. Stores persons and their relations in data structure.      #
# Functions to search data in data structure and print it.                  #
#                                                                           #
# More accurate explanation of parameters in "familytree.hh".               #
#############################################################################
*/

#include <algorithm>
#include "familytree.hh"


Familytree::Familytree()
{
}

Familytree::~Familytree()
{
}

/* Adds a new person pointer to Familytree data. Gets person id, height and
 * output stream as parameters. If the person is already added, an error
 * message is printed.
 */
void Familytree::addNewPerson(const std::__cxx11::string &id, const int &height,
                              std::ostream &output)
{
    if(data_.find(id) == data_.end()) {
        std::shared_ptr<Person> tmp(new Person);
        tmp->id_ = id;
        tmp->height_ = height;

        data_[id] = tmp;
    } else {
        output << "Error. Person already added." << std::endl;
    }
}

/* Prints out all persons that have been added in Familytree data.
 * Gets pointer to output stream as parameter.
 */
void Familytree::printPersons(std::ostream &output) const
{
    for(auto it : data_) {
        output << it.first << ", " << it.second->height_ << std::endl;
    }
}

/* Function gets a name of child and names of parents in a vector as parameters.
 * Parent id validity is checked with function checkId.
 * Parents are added to childs struct and child is added to parents struct.
 */
void Familytree::addRelation(const std::__cxx11::string &child,
                             const std::vector<std::__cxx11::string> &parents,
                             std::ostream &output)
{
    Person* childptr = getPointer(child);
    for(int i = 0; i < 2; ++i) {
        if(parents.at(i) != "-") {
            if(checkId(parents.at(i), output)) {
                Person* parentptr = getPointer(parents.at(i));
                childptr->parents_.at(i) = parentptr;
                parentptr->children_.push_back(childptr);
            }
        }
    }
}

/* Id is checked with function checkId. Children names are stored in set with
 * function IdSet and names/error message are printed with funtion printGroup.
 */
void Familytree::printChildren(const std::__cxx11::string &id,
                               std::ostream &output) const
{
    if(checkId(id, output)) {
        Person* parentptr = getPointer(id);
        IdSet children = VectorToIdSet(parentptr->children_);
        printGroup(id, "children", children, output);
    }
}

/* If id found in Familytree, parents/error message are printed in printGroup
 * function just like in funcion printChildren.
 */
void Familytree::printParents(const std::__cxx11::string &id,
                              std::ostream &output) const
{
    if(checkId(id, output)) {
        Person* childptr = getPointer(id);
        IdSet parents = VectorToIdSet(childptr->parents_);
        printGroup(id, "parents", parents, output);
    }
}

/* If id is valid, function getSiblings is called and return value (set) is
 * stored in variable siblings. printGroup is called to print names/error msg.
 */
void Familytree::printSiblings(const std::__cxx11::string &id,
                               std::ostream &output) const
{
    if(checkId(id, output)) {
        IdSet siblings = getSiblings(id);
        printGroup(id, "siblings", siblings, output);
    }
}

/* printCousins gets id as a parameter and calls function checkId to check if
 * it's valid. Function iterates over target persons parents and calls function
 * getSiblings to get siblings of persons parents which are stored in set.
 * Next for loop iterates over set of parents siblings (which are cousins
 * parents apparently) and calls function VectorToIdSet to get set of cousins.
 * Cousins are then inserted to cousins set and printGroup is called to print
 * names/messages.
 */
void Familytree::printCousins(const std::__cxx11::string &id,
                              std::ostream &output) const
{
    if(checkId(id, output)) {
        Person* idptr = getPointer(id);
        IdSet cousins = {};

        for(auto parent : idptr->parents_) {
            if(parent != nullptr) {
                IdSet parents_siblings = getSiblings(parent->id_);
                for(auto parents_sibling : parents_siblings) {
                    Person* cousins_parent_ptr = getPointer(parents_sibling);
                    IdSet container = VectorToIdSet(cousins_parent_ptr->children_);
                    cousins.insert(container.begin(), container.end());
                }
            }
        }
        printGroup(id, "cousins", cousins, output);
    }
}

/* Function gets id as parameter which is checked with function checkId.
 * Pointer to person with id in question is got as a return value of getPointer.
 * Function printTallestOrShortest is called to do the magic.
 */
void Familytree::printTallestInLineage(const std::__cxx11::string &id,
                                       std::ostream &output) const
{
    if(checkId(id, output)) {
        Person* person = getPointer(id);
        printTallestOrShortest(person, true, output);
    }
}

/* Function works the same way as printTallestInLineage except second parameter
 * in function call printTallestOrShortest has boolean value false.
 */
void Familytree::printShortestInLineage(const std::__cxx11::string &id,
                                        std::ostream &output) const
{
    if(checkId(id, output)) {
        Person* person = getPointer(id);
        printTallestOrShortest(person, false, output);
    }
}

/* Function gets id and integer value as parameters. Value tells how many
 * layers to dig for grandchildren. printGrand is called with given parameters.
 */
void Familytree::printGrandChildrenN(const std::__cxx11::string &id,
                                     const int n, std::ostream &output) const
{
    if(checkId(id, output)) {
        Person* parentptr = getPointer(id);
        printGrand(parentptr, n, true, output);
    }
}

/* printGrandParentsN works just like the function above except printGrand
 * gets false as third parameter.
 */
void Familytree::printGrandParentsN(const std::__cxx11::string &id,
                                    const int n, std::ostream &output) const
{
    if(checkId(id, output)) {
        Person* parentptr = getPointer(id);
        printGrand(parentptr, n, false, output);
    }
}

// Private:

Person *Familytree::getPointer(const std::__cxx11::string &id) const
{
    return data_.at(id).get();
}

/* Gets vector of Person pointers and returns a set of those persons ids.
 */
IdSet Familytree::VectorToIdSet(const std::vector<Person *> &container) const
{
    IdSet new_set = {};
    for(auto person : container) {
        if(person != nullptr) {
            new_set.insert(person->id_);
        }
    }
    return new_set;
}

/* Prints out the given group or message in case the group is empty.
 */
void Familytree::printGroup(const std::__cxx11::string &id,
                            const std::__cxx11::string &group, IdSet &container,
                            std::ostream &output) const
{
    if(container.size() > 0) {
        output << id << " has " << container.size() << " " << group << ":"
               << std::endl;
        for(auto i : container) {
            output << i << std::endl;
        }
    } else {
        output << id << " has no " << group << "." << std::endl;
    }
}

/* Digs recursively through layers of datas tructure in given direction to find
 * grandchilds or -parents. When the needed layer is reached, stores ids to set
 * container.
 */
void Familytree::personsOnLevel(Person * const person, IdSet &container,
                                const bool direction_child, const int &n) const
{
    std::vector<Person* > dir = person->children_;
    if(not direction_child) {
        dir = person->parents_;
    }
    if(n == 1) {
        IdSet tmp = VectorToIdSet(dir);
        container.insert(tmp.begin(), tmp.end());
    } else {
        for(auto it : dir) {
            if(it != nullptr) {
                personsOnLevel(it, container, direction_child, n-1);
            }
        }
    }
}

/* Calls function personsOnLevel and calls printGroup with calculated parameters
 * to print grandchildren and -parents.
 */
void Familytree::printGrand(Person * const person, const int &n,
                            const bool &dir_c, std::ostream &output) const
{
    if(n >= 1) {
        IdSet container = {};
        personsOnLevel(person, container, dir_c, n+1);

        std::string great = "";
        for(int i = 0; i < n-1; ++i) {
            great += "great-";
        }
        std::string group = great+"grandchildren";
        if(not dir_c) {
            group = great+"grandparents";
        }
        printGroup(person->id_, group, container, output);
    } else {
        output << "Error. Level can't be less than 1." << std::endl;
    }
}

/* Calls function heightRecursive to find tallest/shortest person and prints
 * message with right variable values.
 */
void Familytree::printTallestOrShortest(Person * const person,
                                        const bool &tallest,
                                        std::ostream &output) const
{
    Person* personptr = heightRecursive(person, tallest);

    std::string who = person->id_+"'s";
    if(personptr == person) {
        who = "his/her";
    }
    std::string word = "tallest";
    if(not tallest) {
        word = "shortest";
    }
    output << "With the height of " << personptr->height_ << ", "
           << personptr->id_ << " is the " << word << " person in "
           << who << " lineage." << std::endl;
}

/* Gets person pointer and boolean value as parameters. Iterates over persons
 * children and compares their height. Calls recursively itself until tallest or
 * shortest person is found and returns it.
 */
Person *Familytree::heightRecursive(Person * const person,
                                    const bool &tallest) const
{
    Person* tmp = person;

    for(auto it : person->children_) {
        Person* next = heightRecursive(it, tallest);
        if((tmp->height_ < next->height_) == tallest) {
            tmp = next;
        }
    }
    return tmp;
}

/* Checks if given id is valid and returns boolean value. If given id is not in
 * data_ map, prints error message and returns false.
 */
bool Familytree::checkId(const std::__cxx11::string &id,
                         std::ostream &output) const
{
    if(data_.find(id) != data_.end()) {
        return true;
    }
    output << "Error. " << id << " not found." << std::endl;
    return false;
}

/* Gets person id as parameter. Iterates over persons parents to find all their
 * children (persons siblings and step siblings) which are stored in a set.
 * Then removes person in question from set and returns it.
 */
IdSet Familytree::getSiblings(const std::__cxx11::string &id) const
{
    Person* childptr = getPointer(id);
    IdSet siblings = {};
    for(auto parent : childptr->parents_) {
        if(parent != nullptr) {
            IdSet children = VectorToIdSet(parent->children_);
            siblings.insert(children.begin(), children.end());
        }
    }
    siblings.erase(id);
    return siblings;
}

