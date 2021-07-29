#include <iostream>
#include <algorithm>
#include <vector>

/*
 * REF: hackerrank.com/challenges/messages-order/problem
 */

using namespace std;

class Message 
{
public: 
    Message()
		: str{ "empty message!" }
		, id { -1 }
	{}
	Message( string str, int id )
		: str{ str }
		, id{ id }
	{}
    const string& get_text() 
	{
        return str; 
    }
	bool operator<( const Message& msg )
	{
		return id < msg.id;
	}
private:
	string str;
	int id;
};

class MessageFactory 
{
public:
    MessageFactory() {}
    Message create_message( const string& text ) 
	{
    	return Message{ text, int(curr_id++) };    
    }
private:
	unsigned curr_id = 0u;
};


/*---------------------LOCKED SECTION--------------------*/
class Recipient 
{
public:
    Recipient() {}
    void receive( const Message& msg ) 
	{
        messages_.push_back( msg );
    }
    void print_messages() 
	{
        fix_order();
        for (auto& msg : messages_) 
		{
            cout << msg.get_text() << endl;
        }
        messages_.clear();
    }
private:
    void fix_order() 
	{
        sort( messages_.begin(), messages_.end() );
    }
    vector<Message> messages_;
};

class Network 
{
public:
    static void send_messages( vector<Message> messages, Recipient& recipient ) 
	{
		// simulates the unpredictable network, where 
		// sent messages might arrive in unspecified order
        random_shuffle( messages.begin(), messages.end() );         
        for (auto msg : messages) 
		{
			recipient.receive(msg);
        }
    }
};



int main() 
{
    MessageFactory message_factory;
    Recipient recipient;
    vector<Message> messages;
    string text;
	cout << "Press Ctrl+D to finish input!\n";
    while (getline(cin, text)) 
	{
        messages.push_back(message_factory.create_message(text));
    }
    Network::send_messages(messages, recipient);
	cout << "Messages received:\n";
    recipient.print_messages();
}

