#include <iostream>
#include <string>
#include <stack>
using namespace std;

// Display the contents of a stack
void displayStack(stack<string> st){

	if(st.empty()){
		cout << "Stack is empty" << endl;
	}
	else{
		cout << "Stack contents (top to bottom): " << endl;
		while (!st.empty()) {
			cout << st.top() << endl;
			st.pop();
		}
	}
	cout << "----------------------------------------" << endl;
}

/*
	Checking the consistency of XML file

	Errors can be:
		1- Opening a tag without closing it
			Soln: Insert the closing tag

		2- Closing a tag without openeing it
			Soln: Insert the opening tag

		3- Mismatch -> Opening a tag and closing it with a different tag
			Soln: Insert the closing tag for the opened tag
				  and insert the opening tag for the closed tag
*/
void consistency(string xmlFile){

	// Decleration and Initialization
	stack<string> tagStack;
	string s = "";
	int size = xmlFile.length();
	cout << "Size of XML file = " << size << endl;
	cout << "----------------------------------------" << endl;
	int errors=0, line=0;
	
	// Iterate through the XML string
	for(int i=0; i<size; i++){
		
		// Check for opening tag 
		if(xmlFile[i] == '<'&& xmlFile[i+1] != '/'){
			int j = i+1;
			
			// Extract the tag name
			while(xmlFile[j] != '>'){
				s+=xmlFile[j];
				j++;
			}

			cout << "Opening tag: " << s << endl;

			// Push the tag onto the stack
			tagStack.push(s);
			cout << "Pushed: " << tagStack.top() << endl;
			displayStack(tagStack);
			i = j;
			s = "";
		}
		
		// Check for closing tag 
		if(xmlFile[i] == '<' && xmlFile[i+1] == '/'){
			int j = i+2;
			
			// Extract the tag name
			while(xmlFile[j] != '>'){
				s+=xmlFile[j];
				j++;
			}
			
			cout << "Closing tag: " << s << endl;

			// Error(2): Closing a tag without opening it
			// This will be executed if the closed tag is in the last line of the file
			if(tagStack.empty()){
				errors++;
				cout << "Closing a tag without opening it" << endl
					 << "The closed tag is: " << s << endl
					 << "errors = " << errors << endl;
			}
			// Error(3): Mismatch -> Opening a tag and closing it with a different tag
			// This will cover Error(1) and Error(2) if the tags are in the middle of the file
			else if (s != tagStack.top()){
				errors++;
				cout << "Mismatch" << endl
					 << "errors = " << errors << endl;
			}
				
			// Correctly matched pair
			else{
				cout<< "Popped: " << tagStack.top() << endl;
				tagStack.pop();
			}
			
			displayStack(tagStack);
			i = j;
			s = "";
		}
	}
	
	// Error(1): Opening a tag without closing it
	/* if(!tagStack.empty()){
		errors += tagStack.size();
		cout << "Opening a tag without closing it" << endl;
	} */
	
	stack<string> tempStack = tagStack;  // Create a temporary stack to avoid modifying the original stack
    while (!tempStack.empty()) {
        errors++;
        cout << "Error: Opening tag <" << tempStack.top() << "> has no closing tag." << endl;
        tempStack.pop();
    }

	//cout << "----------------------------------------" << endl;
	cout << "Errors = " << errors << endl;
	cout << "Stack size = " << tagStack.size() << endl;
}

int main(){
	string str = "<html>"
				"	bla bla bla"
				"	<body>"				// opening <body> without closing it
				"		2100427"
				"		<name>"			// opening <name> without closing it
				"			stmb"
				"		</id>"			// closing <id> without opening it
				"		<name>"
				"			hello"
				"		</name>"
				"		<name>"			// opening <name> without closing it
				"			world"
				"</footer>"				// closing </footer> without opening it
				"</html>"
				"</script>";			// closing </script> without opening it


	// Error(1) example: Opening a tag without closing it
	/* string str = "<html>"
    			 "	<body>"					// opening <body> without closing it
    			 "		<h1>"
				 "			Title"
				 "		</h1>"
				 "</html>"; */


	// Error(2) example: Closing a tag without opening it
	/* string str = "<html>"
    			 "	<body>"	
    			 "		<h1>"
				 "			Title"
				 "		</h1>"
				 "	</body>"
				 "	</footer>"				// closing </footer> without opening it
				 "</html>";
 */
	

	// Error(3) example: Mismatch
	/* string str = "<html>"
    			 "	<body>"					// opening <body>
        		 "		<h1>Title</h1>"
    			 "	</footer>"				// closing it with </footer>
				 "</html>"; */


	consistency(str);
}