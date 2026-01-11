#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXCHAR 1000
#define LINE_SIZE 1024
// Battery notes! ---
// 3.6 logs per 20 battery -> 3 purple logs and 3 blue logs minimum per combat. -> 0.18 logs per 1 point of battery.
// Agents need 300 logs to cap, that being 900k exp. Purple logs are 3k, Blue logs are 600.


typedef enum {
  stateActive,
  statePassive,
  stateLoadSave
} State;



float investigator_logs_held_purple = 0;
int investigator_logs_held_blue = 0;
int investigator_log_value = 3000;
char exp_progress_flag;

//File paths
char csvFolder[10] = "csv/";
char csvAgentExperience[42] = "csv/agentExperienceMaterials.csv"; 
char csvAgentPromotion[42] = "csv/agentPromotionMaterials.csv";
char csvSkillUpgrade[42] = "csv/skillMaterials.csv";
char csvCoreSkillUpgrade[42] = "csv/coreSkillMaterials.csv";

double pullMaterialsFromSheet(const char *filename, int start_row, int column){


  FILE *file = fopen(filename, "r");
  if (!file){
    perror("failed to open file");
    return 1;
  }

  char line[LINE_SIZE];
  int current_row = 0;
  double sum = 0.0;

  while(fgets(line, sizeof(line), file)){
    if (current_row >= start_row){
      char *token;
      int current_col = 0;

      token = strtok(line, ",");

      while (token){
        if (current_col == column){
          sum += atof(token);
          break;
        }
        token = strtok(NULL, ",");
        current_col++;
      }
    }
    current_row++;
  }
  fclose(file);

  // printf("Sum = %.2f\n", sum);
  return sum;

}

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

struct Material_s{
  char name[30];
  float heldPurple;
  float heldBlue;
  float heldGray;
  int value;
  float conversionRateGrayToBlue;
  float conversionRateBlueToPurple;
  float batteryCost;
}const materialDefault = {
  "dummy",
  0,
  0,
  0,
  0,
  1,
  2,
  0.18
}; typedef struct Material_s defMaterial;

defMaterial investigatorLogs = {
  "Investigator Log",
  0,
  0,
  0,
  3000,
  1,
  5,
  0.18
};
defMaterial skillChips = {
  "Combat Chips",
  0,
  0,
  0,
  1,
  3,
  0
};

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



int convertMaterialToPurple(defMaterial myMaterial){
  myMaterial.heldPurple = (myMaterial.heldBlue/myMaterial.conversionRateBlueToPurple) + myMaterial.heldPurple;
};


int remainingMaterials(defMaterial myMaterial){
  int remainder = ((agent1.required_experience - agent1.current_experience) - (myMaterial.heldPurple * myMaterial.value))/myMaterial.value;
  return remainder;
}

int batteryCostConversion(defMaterial myMaterial){
  float batteryConverted = remainingMaterials(myMaterial)/myMaterial.batteryCost;
  return batteryConverted;

}

int printResults(){
  if(investigatorLogs.heldPurple >= 300) {
    printf("\n");
    printf("=====================================================\n");

    printf("Agent: %s , has completed the required Investigator Logs.\n", agent1.name);
    printf("--- Investigator Logs Data ---\n");
    printf("Battery needed to finish: %.1f \n", batteryCostConversion(investigatorLogs));
    printf("That's %.1f days worth., assuming you spend everything.\n", batteryCostConversion(investigatorLogs)/(240 + 80));
  }
  else {

    printf("\n");
    printf("=====================================================\n");

    printf("Agent: %s , needing a total of %d purple logs to cap.\n", agent1.name, remainingMaterials(investigatorLogs));
    printf("--- Investigator Logs Data ---\n");
    printf("Battery needed to finish: %.1f \n", batteryCostConversion(investigatorLogs));
    printf("That's %.1f days worth., assuming you spend everything.\n", batteryCostConversion(investigatorLogs)/(240 + 80));
  }
  printf("--- Skill Material Data ---\n");
  printf("Skill materials needed:%.f basic chips, %.f mid chips and %.f advanced chips.\n", pullMaterialsFromSheet(csvSkillUpgrade,1,2), pullMaterialsFromSheet(csvSkillUpgrade, 1, 3), pullMaterialsFromSheet(csvSkillUpgrade,1,4) );
  printf("Battery needed to finish: \n");
  printf("That's -- days worth, assuming you spend everything.\n");

  printf("=====================================================\n");
}

int getData(){
  printf("What's the agent's name?: \n");
  scanf("%29s", &agent1.name);
  printf("\n");

  printf("How many Investigator Logs (Purple) do you have?\n");
  scanf("%f", &investigatorLogs.heldPurple);
  printf("\n");
  printf("How many Investiagtor Logs (Blue) do you have?\n");
  scanf("%d", &investigatorLogs.heldBlue);
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


void stateMachine(State *currentState){
  switch (*currentState) {
    case stateActive:
      printf("Active mode. \n");
      getData();
      convertMaterialToPurple(investigatorLogs);
      printResults();
      printf("skill check: %d\n", agent1.ultimate.desired_level);
      break;

    case statePassive:
      printf("Passive mode. \n");
      break;

    default:
      break;
    
  };
}




int main(){
  State myMachineState = stateActive;
  stateMachine(&myMachineState);
}
