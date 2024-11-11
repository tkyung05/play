#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

class TimeManager
{
public:
    static void Init()
    {
        _time = 1;
        _isRunning = true;
    }

    static void Stop()
    {
        _isRunning = false;
    }

    static void Update()
    {
        _time++;
    }

    static int GetTime() { return _time; }
    static int IsRunning() { return _isRunning; }

private:
    static int _time;
    static bool _isRunning;
};

int TimeManager::_time = 0;
bool TimeManager::_isRunning = false;

enum class SkillName
{
    WrappingBandage,
};

class Player;

class Skill
{
public:
    Skill(SkillName name) : _name(name)
    {
    }

    virtual ~Skill()
    {
        _owner = nullptr;
    }

    SkillName GetName() { return _name; }
    Player* GetOwner() { return _owner; }

    void SetOwner(Player* player) { _owner = player; }

    virtual void Activate() {};

private:
    Player* _owner;
    SkillName _name;
};

enum class PlayerStatus
{
    Normal, // 일반
    Silenced, // 침묵 (스킬 사용 불가)
};

class Player
{
public:
    Player(int hp) : _hp(hp), _maxHp(hp), _status(PlayerStatus::Normal)
    {
    }

    ~Player()
    {
        for (auto& skill : _skills)
        {
            if (skill.second == nullptr) continue;

            delete skill.second;
            skill.second = nullptr;
        }
    }

    bool IsDead() { return _hp == 0; }

    int GetHp() { return _hp; }
    PlayerStatus GetStatus() { return _status; }

    void Hit(int damage)
    {
        if (_hp - damage < 0)
        {
            _hp = 0;
            return;
        }

        _hp -= damage;
        SetStatus(PlayerStatus::Silenced);
    }

    void Heal(int amount)
    {
        if (_hp + amount > _maxHp)
        {
            _hp = _maxHp;
            return;
        }

        _hp += amount;
        SetStatus(PlayerStatus::Normal);
    }

    void Learn(Skill* skill)
    {
        _skills[skill->GetName()] = skill;
        skill->SetOwner(this);
    }

    void Activate(SkillName skillName)
    {
        Skill* skill = GetSkill(skillName);

        if (skill != nullptr)
        {
            skill->Activate();
        }
    }

private:
    void SetStatus(PlayerStatus status) { _status = status; }

    bool HasOwn(SkillName skillName) { return _skills.count(skillName); }

    Skill* GetSkill(SkillName skillName)
    {
        if (HasOwn(skillName))
            return _skills.find(skillName)->second;

        return nullptr;
    }

private:
    int _hp;
    int _maxHp;
    PlayerStatus _status;

    unordered_map<SkillName, Skill*> _skills;
};

class WrappingBandage : public Skill
{
    using Super = Skill;
public:
    WrappingBandage(int healAmount, int bonusHealAmount, int maxDuration)
        : Super(SkillName::WrappingBandage),
        _healAmount(healAmount),
        _bonusHealAmount(bonusHealAmount),
        _maxDuration(maxDuration),
        _duration(0)
    {
    }

    void Activate() override
    {
        Player* player = Super::GetOwner();
        int healAmount = _healAmount;

        if (player->GetStatus() == PlayerStatus::Silenced)
        {
            ResetDuration();
        }

        _duration++;
       
        if (_duration == _maxDuration)
        {
            healAmount += _bonusHealAmount;
            ResetDuration();
        }

        player->Heal(healAmount);
    }

private:
    void ResetDuration() { _duration = 0; }

private:
    int _healAmount;
    int _bonusHealAmount;
    int _maxDuration;
    int _duration;
};

class Monster
{
public:
    Monster(int attackDelay, int attackDamage)
        : _attackDelay(attackDelay), _attackDamage(attackDamage)
    {
    }

    int GetAttackDelay() { return _attackDelay; }

    void Attack(Player* target)
    {
        target->Hit(_attackDamage);
    }

private:
    int _attackDelay;
    int _attackDamage;
};

class Field
{
public:
    ~Field()
    {
        if (_player != nullptr)
        {
            delete _player;
            _player = nullptr;
        }
        
        for (Monster* monster : _monsters)
        {
            if (monster == nullptr) continue;
            
            delete monster;
            monster = nullptr;
        }
    }

    static void Enter(Player* player)
    {
        _player = player;
    }
    
    static void Spawn(Monster* monster)
    {
        _monsters.push_back(monster);
    }

    static void Battle()
    {
        int time = TimeManager::GetTime();

        for (Monster* monster : _monsters)
        {
            if (monster->GetAttackDelay() == time)
            {
                monster->Attack(_player);
                return;
            }
        }

        _player->Activate(SkillName::WrappingBandage);
    }

private:
    static Player* _player;
    static vector<Monster*> _monsters;
};

Player* Field::_player = nullptr;
vector<Monster*> Field::_monsters;

int solution(vector<int> bandage_skill_info, int hp, vector<vector<int>> monster_infos)
{
    TimeManager::Init();

    int healAmount = bandage_skill_info[1];
    int bonusHealAmount = bandage_skill_info[2];
    int maxDuration = bandage_skill_info[0];
    WrappingBandage* wrappingBandageSkill = new WrappingBandage(healAmount, bonusHealAmount, maxDuration);

    Player* player = new Player(hp);
    player->Learn(wrappingBandageSkill);

    vector<Monster*> monsters;
    for (vector<int> monsterInfo : monster_infos)
    {
        int attackDelay = monsterInfo[0];
        int attackDamage = monsterInfo[1];
        Monster* monster = new Monster(attackDelay, attackDamage);
        
        monsters.push_back(monster);
    }

    Field::Enter(player);

    for (Monster* monster : monsters)
    {
        Field::Spawn(monster);
    }

    while (TimeManager::IsRunning())
    {
        int time = TimeManager::GetTime();

        Field::Battle();

        if (monsters.back()->GetAttackDelay() == time)
        {
            return player->IsDead() ? - 1 : player->GetHp();
        }

        if (player->IsDead())
        {
            return -1;
        }

        TimeManager::Update();
    }
}