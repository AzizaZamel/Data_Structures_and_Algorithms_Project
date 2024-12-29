#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

// Function to find the most active user from XML string
string most_active_user(const string& xml_content) {
    unordered_map<string, pair<string, int>> user_degrees;

    // Simple parsing logic
    stringstream ss(xml_content);
    string line;
    string current_user_id, current_user_name;

    while (getline(ss, line)) {
        // Check for user opening tag
        if (line.find("<user") != string::npos) {
            size_t id_pos = line.find("id=\"") + 4;
            size_t id_end = line.find("\"", id_pos);
            current_user_id = line.substr(id_pos, id_end - id_pos);

            size_t name_pos = line.find("name=\"") + 6;
            size_t name_end = line.find("\"", name_pos);
            current_user_name = line.substr(name_pos, name_end - name_pos);
            user_degrees[current_user_id] = { current_user_name, 0 }; // Initialize degree
        }

        // Check for connections
        if (line.find("<connection") != string::npos) {
            if (!current_user_id.empty()) {
                user_degrees[current_user_id].second++; // Increment degree
            }
        }
    }

    // Find the user with the highest degree
    string most_active_id, most_active_name;
    int max_degree = -1;
    for (const auto& entry : user_degrees) {
        const string& user_id = entry.first;
        const pair<string, int>& user_data = entry.second;

        if (user_data.second > max_degree) {
            max_degree = user_data.second;
            most_active_id = user_id;
            most_active_name = user_data.first;
        }
    }

    // Return the most active user's name and ID
    if (max_degree >= 0) {
        return "Most Active User: " + most_active_name + ", ID: " + most_active_id;
    }
    else {
        return "No users found in the network.";
    }
}

int main() {
    // XML data as a string
    string xml_content = R"(
<network>
    <user id="1" name="Alice">
        <connections>
            <connection user_id="2" />
            <connection user_id="3" />
        </connections>
    </user>
    <user id="2" name="Bob">
        <connections>
            <connection user_id="1" />
            <connection user_id="3" />
            <connection user_id="4" />
        </connections>
    </user>
    <user id="3" name="Charlie">
        <connections>
            <connection user_id="1" />
            <connection user_id="2" />
            <connection user_id="5" />
        </connections>
    </user>
    <user id="4" name="David">
        <connections>
            <connection user_id="1" />
            <connection user_id="2" />
            <connection user_id="3" />
            <connection user_id="4" />
        </connections>
    </user>
</network>
)";

    // Call the function to find the most active user from the XML string
    cout << most_active_user(xml_content) << endl;

    return 0;
}