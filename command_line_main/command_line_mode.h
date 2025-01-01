/*
 ===================================================================================================
 Name        : command_line_mode.h
 Author      : Doaa Said
 Description : hrader file to program which is designed to handle various operations related to XML files and user
 data. It processes command-line arguments to determine which operation to perform
 Date        : 31/12/2024
 ===================================================================================================
 */

#ifndef COMMAND_LINE_MODE_H_
#define COMMAND_LINE_MODE_H_


 void parseCommand(int argc,char* argv[]);

 void fix_XML_File(const std::string &input_file, const std::string &output_file);

 void checkConsistency_Xml_file(const std::string &input_file);

 void generate_graph(const string& filename,const string& output_file);
#endif /* COMMAND_LINE_MODE_H_ */
