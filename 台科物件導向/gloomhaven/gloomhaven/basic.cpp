#include "Gloom_Haven.h"
Skill::Skill():type(-1), value(-1) {}
Skill::Skill(string name, int val) : value(val) {
	this->set(name, val);
}
Skill::Skill(int in_type, int val) : type(in_type),value(val) {}
void Skill::set(string name, int val) {
	this->value = val;
	string sk_name[4] = { "move" ,"attack" ,"heal" ,"shield" };
	for (int i = 0;i < 4;i++) {
		if (name == sk_name[i]) {
			this->type = i;
			break;
		}
	}
}
Skill::Skill(const Skill& input) {
	*this = input;
}
Skill& Skill::operator=(const Skill& input) {
	this->type = input.type;
	this->value = input.value;
	this->range = input.range;
	this->move_step = input.move_step;
	return *this;
}

//============================================================
Card::Card():skill_up(nullptr),skill_down(nullptr){}
Card::Card(const Card& input) {
	*this = input;
}
Card::~Card() {
	mydelete(skill_up);
	skill_up_amount = 0;
	mydelete(skill_down);
	skill_down_amount = 0;
}
Card& Card::operator=(const Card& input) {
	this->~Card();
	if (input.skill_up_amount > 0 && input.skill_up!=nullptr) {
		this->skill_up_amount = input.skill_up_amount;
		skill_up = new Skill[skill_up_amount];
		for (int i = 0;i < skill_up_amount;i++) {
			skill_up[i] = input.skill_up[i];
		}
	}
	if (input.skill_down_amount > 0 && input.skill_down!= nullptr) {
		this->skill_down_amount = input.skill_down_amount;
		skill_down = new Skill[skill_down_amount];
		for (int i = 0;i < skill_down_amount;i++) {
			skill_down[i] = input.skill_down[i];
		}
	}
	available = input.available;
	discard = input.discard;
	number = input.number;
	agility = input.agility;
	rewash_mark = input.rewash_mark;
	return *this;
}

//==================================================================
Creature::Creature() {}
Creature::~Creature() {
	mydelete(card);
}

int Creature::card_available_amount() {//持有卡牌數(棄牌堆+手牌)
	int count = 0;
	for (int i = 0;i < card_amount;i++) {
		if (card[i].available) {
			count++;
		}
	}
	return count;
}
int Creature::card_hand_amount() {//可用卡牌數
	int count = 0;
	for (int i = 0;i < card_amount;i++) {
		if (!card[i].discard && card[i].available) {
			count++;
		}
	}
	return count;
}
int Creature::card_discard_amount() {//棄牌堆數
	int count = 0;
	for (int i = 0;i < card_amount;i++) {
		if (card[i].discard && card[i].available) {
			count++;
		}
	}
	return count;
}

Card& Creature::find_card(int number) {//int 傳入編號number，回傳該Card reference
	for (int i = 0;i < card_amount;i++) {
		if (card[i].number == number) {
			return card[i];
		}
	}
}

void Creature::heal(int add) {//回血
	if (add < 0) {
		add = 0;
	}
	life_value += add;
	if (life_value > max_life_value) {
		life_value = max_life_value;
	}
	cout << code << " heal " << add << ", now hp is " << life_value << endl;
}

void Creature::discard_to_hand() {//棄牌堆所有牌移回手牌，但不包含已被移除的牌(availible == false)
	for (int i = 0;i < card_amount;i++) {
		if (card[i].available && card[i].discard) {
			card[i].discard = false;
		}
	}
}

void Creature::check_card() {//印出卡牌編號(手牌與棄牌)，編號由小到大
	int max_number=-1;//最大編號
	for (int i = 0;i < card_amount;i++) {
		if (card[i].number > max_number) {
			max_number = card[i].number;
		}
	}
	int min_number = max_number;//最小編號
	for (int i = 0;i < card_amount;i++) {
		if (card[i].number < min_number) {
			min_number = card[i].number;
		}
	}
	int count = 0;
	cout << "hand: ";
	for (int i = min_number;i <= max_number;i++) {//從最小開始跑到最大
		for (int j = 0;j < card_amount;j++) {
			if (card[j].number == i) {
				if (card[j].available && !card[j].discard) {
					cout << (count != 0?", ":"")<< card[j].number;
					count++;
				}
				break;
			}
		}
	}
	count = 0;
	cout << "; discard: ";
	for (int i = min_number;i <= max_number;i++) {//從最小開始跑到最大
		for (int j = 0;j < card_amount;j++) {
			if (card[j].number == i) {
				if (card[j].available && card[j].discard) {
					cout << (count != 0 ? ", " : "") << card[j].number;
					count++;
				}
				break;
			}
		}
	}
	cout << endl;
}


void Creature::shield(int add) {//增加防禦
	cout << code << " shield " << add << " this turn"<<endl;
	TmpShield += add;
	if (TmpShield < 0) {
		TmpShield = 0;
	}
}

void Creature::be_attack(char attacker_code,int attack_val) {//攻擊者會對被攻擊者呼叫此function
	if (attack_val < 0) {
		attack_val = 0;
	}
	cout << attacker_code << " attack " << code << ' ' << attack_val << " damage, ";
	cout << code << " shield " << TmpShield << ", "<<code << " remain ";
	if (TmpShield < attack_val) {
		life_value -= (attack_val - TmpShield);
	}
	cout << life_value << " hp" <<endl;
	if (life_value <= 0) {
		cout << code << " is killed!!" << endl;
		prompt_input(code << " 死亡後的地圖:");
		position.to_null();
		this->map->show_room();
	}
}

void Creature::move(string step,int step_count) {//角色與怪物使用相同移動function
	if (step.length() > step_count && team_num == Team_num::character) {//角色不可超過最大步數，重新輸入
		cout << "error move!!!" << endl;
		this->move(wasd(), step_count);
		return;
	}
	Coord latest_allow = position;
	Coord now = position;
	for (int i = 0;i < step.length();i++) {
		Coord direction[4] = { UP(now),LEFT(now),DOWN(now),RIGHT(now)};
		int dir_index;
		switch (step[i])
		{
		case dir::up:dir_index = 0;break;
		case dir::left:dir_index = 1;break;
		case dir::down:dir_index = 2;break;
		case dir::right:dir_index = 3;break;
		case dir::stay:continue;
		default:break;
		}
		char now_char = map->coord_in_body(direction[dir_index]);
		Creature* now_life = map->creature_in(direction[dir_index]);

		if (now_char == map_obj::m_floor || (team_num != Team_num::monster && now_char == map_obj::door) || now_life == this) {
			latest_allow = direction[dir_index];
			now = direction[dir_index];

		}
		else if (now_life != nullptr) {
			if (now_life->team_num == team_num) {
				now = direction[dir_index];

			}
			else if (now_life->team_num != team_num && team_num == Team_num::character) {//角色不可穿過障礙物或敵人
				cout << "error move!!!" << endl;
				this->move(wasd(),step_count);
				return;
			}
			else if (now_life->team_num != team_num && team_num == Team_num::character) {//怪物撞到敵人
				break;
			}
		}
		else {//剩餘的是障礙物與牆壁
			if (team_num == Team_num::character) {//角色不可穿過障礙物牆壁，怪物則會被擋住
				cout << "error move!!!" << endl;
				this->move(wasd(), step_count);
				return;
			}
			else if (team_num == Team_num::monster) {//怪物撞到障礙物或牆壁就停下(後面的都不做)
				break;
			}
		}
	}
	if (team_num == Team_num::character && now != latest_allow) {//角色最終不可停留在門與地板外的地方
		cout << "error move!!!" << endl;
		this->move(wasd(), step_count);
		return;
	}
	position = latest_allow;
	map->show_room();
}

void Creature::check() {//角色行動前，輸入check，要列出所有角色與怪物的hp與防禦值
	cout << code << "-hp: " << life_value << ", shield: " << TmpShield << endl;
}

bool Creature::in_hand(int number) {//傳入編號，回傳該牌是不是手牌，error handling用
	for (int i = 0;i < card_amount;i++) {
		if (card[i].number == number && card[i].available && !card[i].discard) {
			return true;
		}
	}
	return false;
}

bool Creature::in_discard(int number) {
	for (int i = 0;i < card_amount;i++) {
		if (card[i].number == number && card[i].available && card[i].discard) {//傳入編號，回傳該牌在不在棄牌堆，error handling用
			return true;
		}
	}
	return false;
}

void Creature::print()/*排序後的列印*/ {}
void Creature::action() /*行動*/ {}
void Creature::round_end() /*回合結算階段，該回合結束後的重整(重設數值)*/ {}