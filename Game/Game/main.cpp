#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>
#include <Windows.h>

using namespace std;

//size of field
const int W = 60;
const int H = 20;
//array of field
char field[H][W];
//sprites
char player = 'p', grass = '_', stone = '@', dollar = '$', enemy1 = '&', roof = '^', wall = '|', shop = 'O';
char hospital = '+', npcQD = '!', diamond = '0', npc = 'i', leader = 'L', ninja = '3', soldiers = '8';
char boss = 'B', npc2Q = 'j', Qitem2 = 'h', road = '[', npc3Q = 'g', npc4Q = 'l', plant = '#', water = 'w';
//count of monsters 1lvl
int en1c = 0, en2c = 0, en3c=0;
//coordinates of player
int x = 58, y = 12;
//money
int money = 0, lvl = 0, hp = 100;
//period
int period = 1;
//Quest items
int diamonds = 0;
//is player swiming
bool isSwiwing = false;
//have a player Quest with diamonds?
bool haveDQuest = false;
//in water
bool inWater = false;
//are Boss alive?
bool aliveBoss;
//for second Quest
bool haveQI2 = false, canHaveQ2 = true, haveQ2 = false;
//for third Quest
bool haveQ3 = false, canHaveQ3 = true;
int roadBuilded = 0;
//for firth Quest
bool haveQ4 = false, canHaveQ4 = true;
int cPlants = 0;
string mission = "Find leader";// mission of player
char stay_symbol; // symboll where stay player

//Ninja
struct Ninja
{
	char symbol = ninja;
	int x; int y;
	bool alive = false;
};

Ninja n1;
Ninja n2;
Ninja n3;

void buildField(); // use for create game Field
void displayField(); // use for render frame
char input(); // user for read pressed key
void movePlayer(char k); // move charachter
bool collision(int x, int y);// make research that can player move
void inShop(); // power up player
void fight1(int ex, int ey); // fight with first level monster
void inHospital();//when player collide with +
void npcQDTalk();//talking with npc that have a quest with diamons
void spawnDiamonds();//spawn 5 diamonds in area
void npcTalking();//just talking with NPC
void speakWithLeader(); // speaking with Leader
void enemy1Spawning(); // spawn 1lvl enemys
void enemy2Spawning(); // spawn 2lvl enemys
void enemy3Spawning(); // spawn 3lvl enemys
void fight2(int ex, int ey); // fight with ninjas
void moveNinja(Ninja &n);//moves ninjas
void fight3(int ex, int ey); // fight with soldiers
void bossFight(int ex, int ey); // fight with Boss
void buildQuest(int n); // builds new quests with making missions
void npc2QTalking(); // talk with npc that have quest with gryvnas(can done only one time)
void setColour(int); // set colour for console
void npc3QTalking(); // talking with npc that have quest about building road
void npc4QTalking(); // talking with npc that have quest about planting
int menu(char); // menu
void buildButton(string name, int c); // builds a button for menu

int main()
{
	buildField();
	char key;
	cout << "Press any key to start " << endl;
	int rm = menu('s');
	if (rm == 1)
	{
		//gamecycle
		do
		{
			key = input();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << key << endl;
			if (key != 'm')
			{
				movePlayer(key);
				displayField();
			}
			else
			{
				rm = menu('p');
				if (rm == 2) hp = 0;
			}

		} while (hp>0);
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	cout << "You die!\nPress any key to exit";
	_getch();

	return 0;
}

void buildField()
{
	//filling grass
	for (int i = 0; i < H; i++)
		for (int q = 0; q < W; q++) field[i][q] = grass;
	//spawn player
	field[y][x] = player;
	//spawning money
	field[5][8] = dollar;
	//creating:
	int ey = 13;
	field[ey][45] = roof; field[ey+1][44] = wall; field[ey+1][45] = shop; field[ey+1][46] = wall; //shop
	field[ey][49] = roof; field[ey+1][48] = wall; field[ey+1][49] = hospital; field[ey+1][50] = wall; //hospital
	field[ey][53] = roof; field[ey+1][52] = wall; field[ey+1][53] = npcQD; field[ey+1][54] = wall; //home of NPC with diamond quest
	//spawning villagers
	int ex = 34;
	for (int i = 0; i < 7; i++)
	{
		field[8][ex] = roof; field[9][ex - 1] = wall; field[9][ex] = npc; field[9][ex + 1] = wall;
		ex += 4;
	}
	ex = 34;
	for (int i = 0; i < 7; i++)
	{
		field[17][ex] = roof; field[18][ex - 1] = wall; field[18][ex] = npc; field[18][ex + 1] = wall;
		ex += 4;
	}

	srand(time(NULL));
	int numb = rand() % 5 + 10;
	for(int i = 0; i < numb; i++)
	{
		int ex = rand() % W;
		int ey = rand() % H;
		if (field[ey][ex] == grass) field[ey][ex] = stone;
	}

	//spawn leader
	field[11][50] = leader;
	//lake
	for (int i = 1; i < 6; i++)
	{
		for (int j = 1; j < 6; j++) field[i][j] = water;

	}
}

void displayField()
{
	setColour(28);
	cout << "|Money - " << money << dollar << "||HP - " << hp << "%||Level - "<< lvl << "||Mission - "<< mission << "|"<< endl;
	for (int i = 0; i < H; i++)
	{
		for (int q = 0; q < W; q++)
		{
			//colour displaying
			if (field[i][q] == grass) { setColour(34); }
			else if (field[i][q] == player) 
			{ 
				if (stay_symbol == road) setColour(128);
				if (stay_symbol == water) setColour(144);
				else setColour(32); 
			}
			else if(field[i][q] == leader) { setColour(33); }
			else if (field[i][q] == roof || field[i][q] == wall) {setColour(46); }
			else if (field[i][q] == npc) { setColour(38); }
			else if (field[i][q] == diamond) { setColour(43); }
			else if ((field[i][q] == enemy1 || field[i][q] == ninja) || (field[i][q] == soldiers || field[i][q] == boss)) { setColour(36); }
			else if (field[i][q] == stone) {setColour(39); }
			else if (field[i][q] == dollar || field[i][q] == Qitem2) { setColour( 38); }
			else if (field[i][q] == hospital) { setColour(244); }
			else if ((field[i][q] == npcQD || field[i][q] == npc2Q) || (field[i][q] == npc3Q || field[i][q] == npc4Q)) { setColour(33); }
			else if (field[i][q] == plant) { setColour(38); }
			else if (field[i][q] == road) setColour(135);
			else if (field[i][q] == water) setColour(153);
			else setColour(47);
			cout << field[i][q];
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		cout << "X" << endl;
	}
	for (int i = 0; i < W; i++)
	{
		cout << "X";
	}
	cout << endl;
}

char input()
{
	char act = _getch();
	system("cls");
	return act;
}

void movePlayer(char k)
{
	if (n1.alive) moveNinja(n1);
	if (n2.alive) moveNinja(n2);
	if (n3.alive) moveNinja(n3);
	//field[y][x] = stay_symbol;
	if (stay_symbol == water){inWater = true; field[y][x] = stay_symbol;}
	else 
	{ 
		inWater = false; 
		if (k == 'a' && field[y][x  ] == water)
		{
			//stay_symbol = water;
			field[y][x] = grass;
		}
		else field[y][x] = stay_symbol;
	}
	//getting into the water
	if ((k == 'a'&& x - 1 >= 0) && (stay_symbol == grass && field[y][x - 1] == water)) { x--; player = 'q'; }
	else if ((k == 'd' && x + 1 < W) && (stay_symbol == grass && field[y][x + 1] == water)) { x++; player = 'p'; }


	if ((k == 'w' && y - 1 >= 0) && !(collision(x, y-1))) y--; // up
	else if ((k == 's' && y+1 < H) && !(collision(x, y+1))) y++; // down
	else if ((k == 'a'&& x - 1 >= 0) && (!collision(x - 1, y) && stay_symbol == grass)) { x--; player = 'q'; }//left
	else if ((k == 'd' && x + 1 < W) && (!collision(x + 1, y) && stay_symbol == grass)) { x++; player = 'p';  }//right
	//if swimming
	else if ((k == 'd' && x + 1 < W) && (!collision(x+1, y) && inWater)) { x++; player = 'o'; }
	else if ((k == 'a' && x + 1 < W) && (!collision(x-1, y) && inWater)) { x--; player = 'a'; }
	//quests
	else if ((k == 'b' && field[y][x] != road)&& (y == 11 && (x >= 0 && x <= 38)))
	{
		stay_symbol = road;
		roadBuilded++;
	}
	else if ((k == 'p' && field[y][x] != plant) && ((y <= 19 && y >= 14) && (x >= 22 && x <= 27)))
	{
		stay_symbol = plant;
		cPlants++;
	}
	//breaking stones
	else if (k == 'k' && (field[y][x+1] == stone && player == 'p')) field[y][x+1] = grass;
	else if (k == 'k' && (field[y][x - 1] == stone && player == 'q')) field[y][x - 1] = grass;
	field[y][x] = player;
	/*if (n1.alive) moveNinja(n1);
	if (n2.alive) moveNinja(n2);
	if (n3.alive) moveNinja(n3);*/
}

bool collision(int x, int y)
{
	if (field[y][x] == grass) { stay_symbol = grass;  return false; }
	if (field[y][x] == water) { stay_symbol = water;  return false;  }
	if(field[y][x] == road) { stay_symbol = road;  return false; }
	else if (field[y][x] == plant) { stay_symbol = plant;  return false; }
	else if (field[y][x] == dollar) { money++; return false; } 
	else if (field[y][x] == enemy1) 
	{
		if (lvl < 1) hp -= 10;
		else fight1(x, y); 
		return true; 
	}
	else if (field[y][x] == ninja)
	{
		if (lvl < 2) hp -= 10;
		else fight2(x, y);
		return true;
	}
	else if (field[y][x] == soldiers)
	{
		if (lvl < 3) hp -= 10;
		else fight3(x, y);
		return true;
	}
	else if (field[y][x] == boss)
	{
		if (lvl < 10) hp -= 50;
		else bossFight(x, y);
		return true;
	}
	else if (field[y][x] == shop) { inShop(); return true; }
	else if (field[y][x] == hospital) { inHospital(); return true; }
	else if (field[y][x] == npcQD) { npcQDTalk(); return true; }
	else if (field[y][x] == npc2Q) { npc2QTalking(); return true; }
	else if (field[y][x] == npc3Q) { npc3QTalking(); return true; }
	else if (field[y][x] == npc4Q) { npc4QTalking(); return true; }
	else if (field[y][x] == npc) { npcTalking(); return true; }
	else if (field[y][x] == leader) { speakWithLeader(); return true; }
	else if (field[y][x] == diamond) { diamonds++; return false; }
	else if (field[y][x] == Qitem2) { haveQI2 = true; return false; }
	else return true;
}

void inShop()
{
	char act;
	if(money >= (lvl+1) * 2) 
	{
		if (lvl < 15)
		{
			cout << "Would you power-up? You have enought money(y - yes, n - no). ";
			cin >> act;
			if (act == 'y') { money -= (lvl + 1) * 2; lvl++; cout << "Your level > " << lvl << endl; }
			else cout << "How you whant." << endl;
		}
		else cout << "Sorry, you have max level" << endl;
	}
	else cout << "Sorry, you need " << (lvl+1)*2 - money << dollar << endl;
	_getch();
	system("cls");
}

void fight1(int ex, int ey)
{
	int ehp = 40;
	char act;
	cout << player << " VS " << enemy1 << "\nLets Fight!" << endl;
	while (ehp > 0 && hp > 0)
	{
		srand(time(NULL));
		cout << "Your HP - " << hp << " Enemy HP - " << ehp << endl;
		cout << "Run - r, Fight - f ";
		cin >> act;
		if (act == 'r') { cout << "You`re runned and enemy healed all his hp" << endl; break; }
		else if (act == 'f')
		{
			if (rand() % 2 == 0) { ehp -= lvl * 2 * 10; cout << "Youre hit enemy(-" << 10*lvl*2 << "HP)"<< endl; }
			else { hp -= 10; cout << "Enemy hit you!(-10HP)" << endl; }
		}
	}
	if (ehp <= 0) { cout << "You`re win!" << endl; field[ey][ex] = grass; en1c--; }
	_getch();
	system("cls");

}

void inHospital()
{
	{
		char act;
		if (money >= (100 - hp))
		{
			cout << "Would you power-up your HP? You have enought money(y - yes, n - no). ";
			cin >> act;
			if (act == 'y') { money -= (100 - hp); hp = 100; cout << "You healed fully." << endl; }
			else cout << "How you whant." << endl;
		}
		else cout << "Sorry, you need " << (100 - hp) - money << dollar << endl;
		_getch();
		system("cls");
	}
}

void npcQDTalk()
{
	char act;
	if (!haveDQuest)
	{
		cout << "Do you whant to get Quest(y - yes, n - no)?";
		cin >> act;
		if (act == 'y')
		{
			haveDQuest = true;
			cout << "Please, collect my gems.\n I lost them, when walking.\nThat was 5 diamonds - "<< diamond << endl;
			spawnDiamonds();
		}
		else cout << "How do you whant." << endl;
	}
	else
	{
		if (diamonds < 5) cout << "You havent 5 diamonds. You need only " << 5 - diamonds << endl;
		else
		{
			cout << "Thanks very much! Here are you salary - +5$" << endl;
			money += 5;
			haveDQuest = false;
			diamonds = 0;
		}
	}
	_getch();
	system("cls");
}

void spawnDiamonds()
{
	field[6][19] = diamond;
	field[8][21] = diamond;
	field[12][26] = diamond;
	field[5][19] = diamond;
	field[12][27] = diamond;
}

void npcTalking()
{
	char act;
	char GB[] = "\nV:That was good Talk! Have a nice day!";
	cout << "V:Hy!\n*****\nYour Answer:\n|1 - Hello, tell me please something.|\n|2 - Hello.|\n|another - Goodbye!|" << endl;
	//cin.get();
	if (!(cin >> act)) { cout << "Villager doesn`t know, what you are saying." << endl; npcTalking(); }
	else
	{
		if (act == '1')
		{
			cout << "Y:Hello, tell me please something\nV:What?\n|1 - about village|\n|another - Nothing, Goodbye!|" << endl;
			if (!(cin >> act)) { cout << "Villager doesn`t know, what you are saying." << endl; npcTalking(); }
			else if(act == '1')
			{
				cout << "Y: About Village\nV:Its very old villge.Nowho dont know, whet it builded\nY:Thanks, goodbye!" << GB << endl;
			}
			else cout << "Y:Nothing, goodBye" << GB << endl;
		}

		else if (act == '2')
		{
			cout << "Y:Hello!\nV:How are you?\n|1 - good|\n|another - bad|" << endl;
			if (!(cin >> act)) { cout << "Villager doesn`t know, what you are saying." << endl; npcTalking(); }
			else
			{
				if (act == '1')
				{
					cout << "Y:Good\nV: Me too. Sorry, i must run.\nY:Goodbye!" << GB << endl;
				}
				else
				{
					cout << "Y:Bad\nV:I know, what will you help\nGues, how many place will be free after Big Britain left Europe? 1GB!\nOh, Sorry, i must run.\nY:Goodbye!" << GB << endl;
				}
			}
			
		}
		else cout << "Y:GoodBye" << GB << endl;
	}
	_getch();
	system("cls");
}

void speakWithLeader()
{
	if (mission == "Find leader")
	{
		cout << "L:Please, help our village!\nThe monsters occupate our cordons.\nWe cant speaking with another villages\n";
		cout << "Y:What i must do?\nL:Kill them.\nY:Ok, i will do that.";
		enemy1Spawning();
		mission = "Kill monsters";
	}
	else if (mission == "Kill monsters")
	{
		if (en1c <= 0)
		{
			cout << "L:Thanks so much!" << endl;
			money += 60;
			cout << "+60$\nNew Quest unlocked!" << endl;
			buildQuest(1);
			cout << "L:Oh no, near village are ninjas.\nY : Ok, i will help you.";
			mission = "Kill ninjas";
			enemy2Spawning();
		}
		else { cout << "L:You need kill " << en1c << " monsters."; }
	}
	else if (mission == "Kill ninjas")
	{
		if (en2c <= 0)
		{
			cout << "L:Thanks so much!" << endl;
			money += 80;
			cout << "+80$" << endl;
			enemy3Spawning();
			cout << "New quest unlocked!" << endl;
			buildQuest(2);
			mission = "Kill soldiers";
			cout << "L:It`s they.\nY:Who?\nV:Soldiers.\nY:I`ll help you\n";
		}
		else { cout << "L:You need kill " << en2c << " ninjas."; }
	}

	else if (mission == "Kill soldiers")
	{
		if (en3c <= 0)
		{
			cout << "L:Thanks so much!" << endl;
			money += 100;
			cout << "New quest avaible" << endl;
			buildQuest(3);
			cout << "+100$\nL:O, here is boss of this organisation.\nY:I`m tired!..Ok, but last time!" << endl;
			mission = "Kill Boss";
			field[10][15] = boss;
			aliveBoss = true;
		}
		else { cout << "L:You need kill " << en3c << " soldiers.";}
	}
	else if (mission == "Kill Boss")
	{
		if (aliveBoss) cout << "He`s not dead now." << endl;
		else
		{
			cout << "Thanks very much for help! We will never forgot this!\n+1000$" << endl;
			money += 1000;
			mission = "Have Fun!";
		}
	}
	_getch();
	system("cls");
}

void enemy1Spawning()
{
	int X = 10, Y = 12;
	for (int i = 1; i <= 4; i++)
	{
		field[Y + i][X + i*2] = enemy1;
	}
	en1c = 4;
}

void enemy2Spawning()
{
	en2c = 2;
	n1.x = 15; n1.y = 10;
	n2.x = 16; n2.y = 14;
	n3.x = 17; n3.y = 18;
	n1.alive = true; n2.alive = true; n3.alive = true;
	field[n1.y][n1.x] = ninja;
	field[n2.y][n2.x] = ninja;
	field[n3.y][n3.x] = ninja;
}

void fight2(int ex, int ey)
{
	int ehp = 60;
	char act;
	cout << player << " VS " << ninja << "\nLets Fight!" << endl;
	while (ehp > 0 && hp > 0)
	{
		srand(time(NULL));
		cout << "Your HP - " << hp << " Enemy HP - " << ehp<< endl;
		cout << "Run - r, Fight - f ";
		cin >> act;
		if (act == 'r') { cout << "You`re runned and enemy healed all his hp" << endl; break; }
		else if (act == 'f')
		{
			if (rand() % 2 == 0) { ehp -= lvl * 2 * 10; cout << "Youre hit enemy(-" << 10 * lvl * 2 << "HP)" << endl; }
			else { hp -= 15; cout << "Enemy hit you!(-15HP)" << endl; }
		}
	}
	if (ehp <= 0) 
	{ 
		cout << "You`re win!" << endl; 
		field[ey][ex] = grass; 
		en2c--; 
		if (n1.x == ex && n1.y == ey)
		{
			n1.alive = false;
		}
		else if (n2.x == ex && n2.y == ey)
		{
			n2.alive = false;
		}
		else if (n3.x == ex && n3.y == ey)
		{
			n3.alive = false;
		}
	}
	_getch();
	system("cls");

}

void moveNinja(Ninja &n)
{
	field[n.y][n.x] = grass;
	switch(period)
	{
	case 1: n.x++; period++;  break;
	case 2: n.y++; period++;  break;
	case 3: n.x--; period++;  break;
	case 4: n.y--; period = 1; break;
	}
	field[n.y][n.x] = ninja;
}

void enemy3Spawning()
{
	int X = 10, Y = 8;
	for (int i = 1; i <= 5; i++)
	{
		field[Y + i*2][X] = soldiers;
	}
	en3c = 5;
}

void fight3(int ex, int ey)
{
	int ehp = 80;
	char act;
	cout << player << " VS " << soldiers << "\nLets Fight!" << endl;
	while (ehp > 0 && hp > 0)
	{
		srand(time(NULL));
		cout << "Your HP - " << hp << " Enemy HP - " << ehp << endl;
		cout << "Run - r, Fight - f ";
		cin >> act;
		if (act == 'r') { cout << "You`re runned and enemy healed all his hp" << endl; field[ey - 1][ex - 1] = soldiers;  en3c++;  break; }
		else if (act == 'f')
		{
			if (rand() % 2 == 0) { ehp -= lvl * 2 * 10; cout << "Youre hit enemy(-" << 10 * lvl * 2 << "HP)" << endl; }
			else { hp -= 20; cout << "Enemy hit you!(-20HP)" << endl; }
		}
	}
	if (ehp <= 0) { cout << "You`re win!" << endl; field[ey][ex] = grass; en3c--; }
	_getch();
	system("cls");
}

void bossFight(int ex, int ey)
{
	int bosshp = 900;
	int tmp = 0;
	bool defence;
	cout << "Final Fight!\n" << player << " VS " << boss << "Lets go!" << endl;
	srand(time(NULL));
	char act;
	while (bosshp > 0 && hp > 0)
	{
		cout << "You: " << hp << "HP vs BOSS " << bosshp << "HP\nf - fight, s - use shield: " << endl;
		cin >> act;
		tmp = rand() + 1;
		if (act == 's') defence = true;
		else if (act == 'f') defence = false;
		if (tmp%2 == 0 || defence)
		{
			if (tmp%2 == 0) cout << "Boss defeanded with his shield" << endl;
			else cout << "You saved by shield" << endl;
		}
		else
		{
			tmp = rand() + 1;
			if (tmp%2 == 0) { cout << "You damaged boss(-" << lvl * 2 * 10 << "HP)!" << endl; bosshp -= lvl * 2 * 10; }
			else { cout << "You damaged boss(-" << lvl * 2 * 10 << "HP) and boss damaged you(-25HP)!" << endl; bosshp -= lvl * 2 * 10; hp -= 25; }
		}
	}
	if (bosshp <= 0) { cout << "You`re winned!" << endl; field[ey][ex] = grass;  aliveBoss = false; _getch(); system("cls"); }
}

void buildQuest(int n)
{
	if (n == 1) 
	{ 
		field[3][57] = roof; 
		field[4][56] = wall; 
		field[4][57] = npc2Q; 
		field[4][58] = wall; 
		field[1][59] = stone; 
	}
	else if(n == 2) 
	{ 
		field[11][38] = road; 
		field[12][37] = roof; 
		field[13][36] = wall; 
		field[13][37] = npc3Q; 
		field[13][38] = wall;
	}
	else if (n == 3) 
	{ 
		field[19][27] = plant; 
		field[14][22] = plant;
		field[17][29] = roof;
		field[18][28] = wall;
		field[18][29] = npc4Q;
		field[18][30] = wall;
	}
}

void npc2QTalking()
{
	char act;
	if (canHaveQ2)
	{
		if (!haveQ2)
		{
			cout << "Dou you whant to get a quest?(y - yes, n - no) ";
			cin >> act;
			if (act == 'y')
			{
				haveQ2 = true;
				field[0][58] = enemy1; field[0][59] = Qitem2;
				cout << "J:Hear my home is money from Ukraine - " << Qitem2 << " hryvnas." << endl;
				cout << "Monster robbed that and now defend it. Please, help me.\nY:Ok" << endl;
			}
			else cout << "How do you whant" << endl;
		}
		else
		{
			if (haveQI2)
			{
				cout << "Thank you very much! I think, you can get some money.\n+30$" << endl;
				money += 30;
				canHaveQ2 = false;
			}
			else cout << "Please, bring this" << endl;
		}
	}
	else cout << "Thanks for that help!" << endl;
	_getch();
	system("cls");
}

void setColour(int i)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i);
}

void npc3QTalking()
{
	char act;
	if (canHaveQ3)
	{
		if (!haveQ3)
		{
			cout << "Dou you whant a quest(y - yes, n - no) ";
			cin >> act;
			if (act == 'y')
			{
				field[11][38] = road;
				cout << "G:So, we need build road\nY:Ok, from where i must start?\nG:Do you see start?\n";
				cout << "Y:Yes\nG:From there start, and go tho left end\nY:Ok\nG:Press b to build" << endl;
				haveQ3 = true;
			}
			else cout << "How you whant" << endl;
		}
		else
		{
			if (roadBuilded >= 38)
			{
				cout << "G:That road are beautiful!\nY:I tried\nG:Thatks so much! Your salary : \n+40$" << endl;
				money += 40;
				canHaveQ3 = false;
			}
			else cout << "You need build " << 39 - roadBuilded << " points of road" << endl;
		}
	}
	else cout << "That road builded by you. Remember that!" << endl;
	_getch();
	system("cls");
}

void npc4QTalking()
{
	char act;
	if (canHaveQ4)
	{
		if (!haveQ4)
		{
			cout << "Do you whant to get a quest?(y - yes, n - no)" << endl;
			cin >> act;
			if (act == 'y')
			{
				cout << "Y:What i should do?\nl:Do you see this plants?\nY:Yes\nl:Plant a plants in this zone\nY:Ok" << endl;
				haveQ4 = true;
			}
			else cout << "How do you whant." << endl;
		}
		else
		{
			if (cPlants < 34) cout << "l:You need plant only " << 34 - cPlants << " plants." << endl;
			else
			{
				cout << "l:Thanks so much!We`ll always knowing about you!\n+50$" << endl;
				money += 50;
				canHaveQ4 = false;
			}
		}
	}
	else cout << "With your help our village have enought food for selling. Thanks You!" << endl;
	_getch();
	system("cls");
}

int menu(char p)
{
	int result = 1;
	char act;
	int etp = 1;
	system("cls");
	if (p == 's')
	{
		while (1)
		{
			setColour(209);
			cout << "In the Village\nw - choose upper button, s - downer, c - click" << endl << endl;
			if (etp == 1) buildButton("Play", 30);
			else buildButton("Play", 62);
			cout << endl;
			if (etp == 2) buildButton("Info", 30);
			else buildButton("Info", 62);
			cout << endl;
			if (etp == 3) buildButton("Exit", 30);
			else buildButton("Exit", 62);
			cout << endl;
			act = _getch();
			system("cls");
			if (act == 'w' && etp > 1) etp--;
			else if (act == 's' && etp < 3) etp++;
			else if (act == 'c' && (etp == 1 || etp == 3)) { return etp; break; }
			else if (act == 'c' && etp == 2)
			{
				system("cls");
				setColour(224);
				cout << "Velcome to the 'In the Village'!" << endl;
				cout << "Move:\nw, s, a, d - WITHOUT CAPLSLOCK!\nFight:\n" << enemy1 << " - from 1 lvl.\n";
				cout << ninja << " - from 2 lvl\n" << soldiers << " - from 3 lvl\nBoss - " << boss << " - from 10 lvl\n";
				cout << "Levels:\nEvery level cost - lvl*2 " << dollar << "\nDamage:\nDamage = lvl * 10 * 2\n";
				cout << "Quests:\nEvery npc with ";
				setColour(1);
				cout << "blue ";
				setColour(224);
				cout << "colour has a quest\nPause:\nJust click m when play.\n";
				setColour(7);
				_getch();
				system("cls");
			}

		}

	}
	else if (p == 'p')
	{
		while (1)
		{
			setColour(13);
			cout << "Pause" << endl << endl;
			if (etp == 1) buildButton("Continue", 30);
			else buildButton("Continue", 62);
			cout << endl;
			if (etp == 2) buildButton("Exit", 30);
			else buildButton("Exit", 62);
			cout << endl;
			act = _getch();
			system("cls");
			if (act == 'w' && etp > 1) etp--;
			else if (act == 's' && etp < 2) etp++;
			else if (act == 'c')
			{
				if (etp == 2) return etp;
				break;
			}
		}
	}
	return result;
}

void buildButton(string name, int c)
{
	for (int i = 0; i < 5; i++)
	{
		setColour(c);
		if (i == 2)
		{
			int numb = 15 - name.length();
			for (int j = 0; j < numb / 2; j++) cout << " ";
			cout << name;
			for (int j = 0; j < (15 - name.length()) - numb/2; j++) cout << " ";
		}
		else
		{
			for (int j = 0; j < 15; j++) cout << " ";
		}
		cout << endl;
		setColour(7);
	}
}