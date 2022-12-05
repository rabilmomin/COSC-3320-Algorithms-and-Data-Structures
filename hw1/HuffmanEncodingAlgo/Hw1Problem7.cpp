#include <iostream> 
#include <sstream>
#include <queue>
#include <map>
#include <time.h>

using namespace std;

template<typename Key, typename Value>
void printMap(map<Key,Value> ourMap) // printing our freqMap and bitCodeMap
{
	for (auto iter = ourMap.begin(); iter != ourMap.end(); iter++)
	{
		cout << iter->first << " : " << iter->second << endl;
	}
}

string encodeMessage(map<char, string> bitCodeMap, string message) // parse the message and create a new encoded string that represent's our string in huffman code
{
	string encoded = "";
	for (int k = 0; k < message.size(); k++)
	{
		encoded += bitCodeMap[message[k]] + " ";
	}
	return encoded;
}

struct CharacterNode // This struct functions similarly to a binary tree
{
	char character;
	int freq; // frequency of character determined from the freqMap
	bool sumNode; // determines if this node contains a character or if it is the sum of two children nodes
	string bitCode; // huffman bitcode


	CharacterNode* left; // children nodes
	CharacterNode* right;

	CharacterNode(char _character, int _freq) // constructor for a specific character
	{
		right = NULL;
		left = right; //
		character = _character;
		freq = _freq;
		sumNode = false;
	}
	CharacterNode(bool _sumNode, int _freq) // constructor for a node that contains the sum of its leaves
	{
		right = NULL;
		left = right;
		freq = _freq;
		_sumNode = true;
	}

};

class characterComparator // comparator that prioritzes nodes that have a smaller frequency used in our priority queue to order freq from least to greatest
{
public:
	int operator()(CharacterNode* x, CharacterNode* y)
	{
		return x->freq > y->freq;
	}
};


void assignCodes(CharacterNode* node, string str, map<char, string> &bitCodeMap) // assigns the correct code to each character in the HuffmanTree and bitCodeMap
{
	if (node)
	{
		if (node->sumNode == false)
		{
			node->bitCode = str;
			bitCodeMap[node->character] = str;
		}
		assignCodes(node->left, str + "0", bitCodeMap);
		assignCodes(node->right, str + "1", bitCodeMap);
	}
}


void deleteHuffmanTree(CharacterNode* node) // clear the memory that is being used by our the huffman tree
{
	if (!node)
	{
		return;
	}
	else
	{
		deleteHuffmanTree(node->left);
		deleteHuffmanTree(node->right);
		delete(node);
	}
}

CharacterNode* HuffmanTree(map<char, int> freqMap)
{
	struct CharacterNode* left;
	struct CharacterNode* right;
	struct CharacterNode* root;

	characterComparator temp;

	priority_queue<CharacterNode*, vector<CharacterNode*>, characterComparator>* prioQueue =  
		new priority_queue< CharacterNode*, vector<CharacterNode*>, characterComparator>(); 

	for (auto iter = freqMap.begin(); iter != freqMap.end(); iter++) // push all values from freqMap onto our priority queue
	{
		prioQueue->push(new CharacterNode(iter->first, iter->second));
	}

	while (prioQueue->size() != 1) // loop function until queue contains one element, the root of the entire huffman tree
	{

		left = prioQueue->top(); // take the smallest values from the queue
		prioQueue->pop();
		right = prioQueue->top();
		prioQueue->pop();


		root = new CharacterNode(true, left->freq + right->freq); // create a root that contains the sum of the frequencies

		root->left = left; // assign left and right as children of root
		root->right = right;

		prioQueue->push(root); // push root back into priority queue
	}
	CharacterNode* result = prioQueue->top(); 

	prioQueue->empty(); // empty and free the memory in use by the queue
	delete(prioQueue);

	return result;
}

int main()
{
	
	map<char, int> freqMap;
	map<char, string> bitCodeMap;
	string message;
	string encodedMsg;
	CharacterNode* huffmanTree;

	cout << "Input message: ";
	getline(cin, message);
	cout << endl;
	int length = message.size();

	for (int k = 0; k < length; k++) // parses through string puts its value in the freq map
	{

		if (freqMap[message[k]] == NULL)
		{
			freqMap[message[k]] = 1;
		}
		else
		{
			freqMap[message[k]]++;
		}
	}
	
	cout << "Character Frequency" << endl;
	printMap(freqMap);
	cout << endl;
	huffmanTree = HuffmanTree(freqMap);

	assignCodes(huffmanTree,"", bitCodeMap);

	cout << "Character Bit Code" << endl;
	printMap(bitCodeMap);
	cout << endl;

	cout << "Encoded Message" << endl;
	encodedMsg = encodeMessage(bitCodeMap, message);
	cout << encodedMsg << endl;
	cout << endl;

	deleteHuffmanTree(huffmanTree);
	
	
}