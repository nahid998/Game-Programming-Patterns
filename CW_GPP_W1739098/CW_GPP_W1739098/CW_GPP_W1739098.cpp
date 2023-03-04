#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

class Card;

void shuffle(vector<Card*>& cards) {
	srand((int)time(NULL));
	for (int i = (int)cards.size() - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap(cards[i], cards[j]);
	}
}

class Player {
protected:
	Player* opponent;
	int health; //health is shared between all players
	vector<Card*> deck, hand, discard_pile;
public:
	Player() {
		health = 20;
	}

	void setOpponent(Player* p) {
		opponent = p;
	}

	int getHealth() {
		return health;
	}

	vector<Card*>& getDeck() {
		return deck;
	}

	void loseHealth(int i) {
		health -= i; // It handle damages
	}

	void resetHealth() {
		this->getHealth();

	}
	void checkTopCard(Card* c) {
	}

	void addToDeck(Card* c) {
		deck.push_back(c);
		::shuffle(deck);
	}

	bool drawCard() {
		if (!deck.empty()) {
			hand.push_back(deck.back());
			deck.pop_back();
			return true;
		}
		return false;
	}

	void playCard(Card* c) {
		c = hand.back();
		discard_pile.push_back(hand.back());
		hand.pop_back();
	}

	virtual void myTurn() = 0;

	bool hasLost() {
		return (deck.empty() || health <= 0); // if one of the player's health reach 0 or finish the cards the game finish this function is completed in the Main Function
	}

	Player* nextTurn() { // it manage the turns
		return opponent;
	}

	void discardToDeck() {
		::shuffle(discard_pile);
		for (auto i : discard_pile) {
			deck.push_back(i);
		}
		discard_pile.clear();
	}

	void shuffle() { //randomize the decks
		::shuffle(deck);
	}
};

class Card { // card class shared with other subclasses
public:
	virtual string getName() = 0;
	virtual void effect(Player* p) = 0;
};

class PainfulLesson : public Card {
public:
	string getName() {
		return "Painful lesson"; // this string will be shown if this card is in your hand
	}

	void effect(Player* p) {
		p->loseHealth(2); //2 points damage to health
		p->drawCard(); //after using the card it will automatically draw a new card for the player
	}
};
class Spite : public Card {
public:
	string getName() {
		return "Spite";
	}

	void effect(Player* p) {
		p->loseHealth(1); //spite doesn't make damage to both opponent i tried to use this() function to make damage but it did not work
	}
};
class FullHeal : public Card {
public:
	string getName() {
		return "Full heal";
	}

	void effect(Player* p) {
		p->resetHealth(); // It will restore your health to 20
	}
};
class Switcheroo : public Card {
public:
	string getName() {
		return "Switcheroo";
	}

	void effect(Player* p) {
		p->shuffle();

	}
};
class Refresh : public Card {
public:
	string getName() {
		return "Refresh";
	}

	void effect(Player* p) {
		p->loseHealth(3);
		p->discardToDeck();
	}
};
class Peek : public Card {
public:
	string getName() {
		return "Peek";
	}

	void effect(Player* p) {
		//tried differents string function, but none of them seem to work
	}
};

class HumanPlayer : public Player {
public:
	void myTurn() {
		cout << "Human health: " << getHealth() << endl; // this part shows your HP every turn
		cout << "Your deck:" << endl;
		for (int i = 0; i < hand.size(); i++) {
			cout << hand[i]->getName() << endl;
		}
		string choice;

		while (choice != "q") {
			cout << "Enter the card you want to play or 'q' to end your turn: ";
			getline(cin, choice);
			if (choice == "q")
				return;

			for (int i = 0; i < hand.size(); i++) {
				if (choice == hand[i]->getName()) {
					hand[i]->effect(opponent);
					discard_pile.push_back(hand[i]);
					for (int j = i; j < hand.size() - 1; j++) {
						hand[j] = hand[j + 1];
					}
					hand.pop_back();

					drawCard();
					return;
				}
				else if (i == hand.size() - 1) {
					cout << choice << " Card not in deck" << endl;
				}
			}
		}
	}
};

class ComputerPlayer : public Player {
public:
	void myTurn() {
		cout << endl << "Computer health: " << getHealth() << endl;
		int i = rand() % hand.size();

		cout << "Card played: " << hand[i]->getName() << endl;
		hand[i]->effect(opponent);
		discard_pile.push_back(hand[i]);
		remove(hand.begin(), hand.end(), hand[i]);
		cout << endl;
	}
};

int main() {
	HumanPlayer* hp = new HumanPlayer;
	ComputerPlayer* cp = new ComputerPlayer;

	for (int i = 1; i <= 5; i++) {
		hp->addToDeck(new PainfulLesson);
		cp->addToDeck(new PainfulLesson);
	}

	for (int i = 1; i <= 6; i++) {
		hp->addToDeck(new Spite);
		cp->addToDeck(new Spite);
	}

	for (int i = 1; i <= 2; i++) {
		hp->addToDeck(new FullHeal);
		cp->addToDeck(new FullHeal);
	}

	for (int i = 1; i <= 2; i++) {
		hp->addToDeck(new Switcheroo);
		cp->addToDeck(new Switcheroo);
	}

	for (int i = 1; i <= 2; i++) {
		hp->addToDeck(new Refresh);
		cp->addToDeck(new Refresh);
	}

	for (int i = 1; i <= 2; i++) {
		hp->addToDeck(new Peek);
		cp->addToDeck(new Peek);
	}

	for (int i = 1; i <= 3; i++) {
		hp->shuffle();
		cp->shuffle();
		hp->drawCard();
		cp->drawCard();
	}

	hp->setOpponent(cp);
	cp->setOpponent(hp);

	Player* turn = hp;

	while (!turn->hasLost()) {
		turn->myTurn();
		turn = turn->nextTurn();
	}

	if (turn == hp) {
		cout << "Computer has won!" << endl;
	}
	else {
		cout << "Human has won!" << endl;
	}
}