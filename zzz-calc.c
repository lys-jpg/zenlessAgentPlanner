#include <stdio.h>

// Battery notes! ---
// 3.6 logs per 20 battery -> 3 purple logs and 3 blue logs minimum per combat. -> 0.18 logs per 1 point of battery.
// Agents need 300 logs to cap, that being 900k exp. Purple logs are 3k, Blue logs are 600.

float investigator_logs_held_purple = 0;
int investigator_logs_held_blue = 0;
int investigator_log_value = 3000;
char exp_progress_flag;


struct Skill_s{
  int level,max_level,desired_level;
} const Skill_default = { 1, 12, 11};

typedef struct Skill_s Skill;


struct Agent_s{
  char name[30];
  int required_experience;
  int level;
  int current_experience;
  Skill attack;
  Skill dash; 
  Skill parry;
  Skill special;
  Skill ultimate;
}const Agent_default = { .name = "Anby", .required_experience = 900000, .level = 1, .current_experience = 0, .attack = Skill_default, .dash = Skill_default, .parry = Skill_default, .special = Skill_default, .ultimate = Skill_default };
typedef struct Agent_s Agent;

Agent agent1 = Agent_default;


struct Wengine_s{
  int level;
  int max_level;
  int required_experience;
  int current_experience;
  int required_materials;
  int current_materials;
}const Wengine_default = { 1, 60, 600000, 0, 0, 0};
typedef struct Wengine_s Wengine;

Wengine wengine1 = Wengine_default;



int blueLogstoPurpleLogs(){
  investigator_logs_held_purple = (investigator_logs_held_blue/5) + investigator_logs_held_purple;
}


int remainingLogs(){
  int remainder = ((agent1.required_experience - agent1.current_experience) - (investigator_logs_held_purple * investigator_log_value))/investigator_log_value;
  return remainder;
} 

int printResults(){
  printf("\n");
  printf("-----------------------------------------------------\n");
  printf("Agent: %s , needing a total of %d purple logs to cap.\n", agent1.name, remainingLogs());
  printf("--- Investigator Logs Data ---\n");
  printf("Battery needed to finish: %.1f \n", remainingLogs()/0.18);
  printf("That's %.1f days worth.\n", (remainingLogs()/0.18)/(240 + 80));

}

int getData(){
  printf("What's the agent's name?: \n");
  scanf("%29s", &agent1.name);
  printf("\n");

  printf("How many Investigator Logs (Purple) do you have?\n");
  scanf("%f", &investigator_logs_held_purple);
  printf("\n");
  printf("How many Investiagtor Logs (Blue) do you have?\n");
  scanf("%d", &investigator_logs_held_blue);
  printf("\n");

  printf("What's the agent's current level? (1~59) \n");
  scanf("%d", &agent1.level);
  printf("\n");

  printf("Do they have any EXP progress on that level? (y/n) \n");
  scanf(" %c", &exp_progress_flag);
  if (exp_progress_flag == 'y'){
    printf("How much?\n");
    scanf("%d", &agent1.current_experience);
  }
  else {
    printf("\n");
  }
}

int main(){
  getData();
  blueLogstoPurpleLogs();
  printResults();
  printf("skill check: %d\n", agent1.ultimate.desired_level);
}
