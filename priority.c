#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int id;
  int arrival_time;
  int burst_time;
  int priority;
  int wait_time;
  int turnaround_time;
  int finished;
} process;

int main(int argc, char **argv) {

  int display_info = 0;
  int display_sequence = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--display-info") == 0)
      display_info = 1;
    else if (strcmp(argv[i], "--display-sequence") == 0)
      display_sequence = 1;
    else {
      printf("\033[91mUnknown Parameter `%s`\033[0m\n", argv[i]);
      return -1;
    }
  }

  int proc_count;
  scanf("%d", &proc_count);

  if (proc_count <= 0) {
    printf("\033[91mprocess count must be >= 1\033[0m\n");
    return -1;
  }

  process *proc_list = malloc(proc_count * sizeof(process));
  process *schedule = malloc(proc_count * sizeof(process));
  //input buffer limited only by the power of scanf and GLIBC
  char *buff = malloc(1024);

  if (!proc_list || !schedule || !buff) {
    printf("\033[91mMalloc Error\033[0m\n");
    return -1;
  }

  for (int i = 0; i < proc_count; i++) {

    scanf("%1023s%d%d%d", buff, &proc_list[i].arrival_time,
          &proc_list[i].burst_time, &proc_list[i].priority);

    if (buff[0] == 'P' || buff[0] == 'p')
      proc_list[i].id = atoi(buff + 1);
    else
      proc_list[i].id = atoi(buff);

    proc_list[i].wait_time = 0;
    proc_list[i].turnaround_time = 0;
    proc_list[i].finished = 0;
  }

  free(buff);
  //input complete

  int completed = 0;
  int t = 0;

  while (completed < proc_count) {

    int selected = -1;

    for (int i = 0; i < proc_count; i++) {

      if (!proc_list[i].finished && proc_list[i].arrival_time <= t) {

        if (selected == -1 ||
            proc_list[i].priority < proc_list[selected].priority) {

          selected = i;
        }
      }
    }

    if (selected == -1) {
      t++;
      continue;
    }

    proc_list[selected].wait_time = t - proc_list[selected].arrival_time;

    proc_list[selected].turnaround_time =
        proc_list[selected].wait_time + proc_list[selected].burst_time;

    schedule[completed] = proc_list[selected];

    t += proc_list[selected].burst_time;

    proc_list[selected].finished = 1;
    completed++;
  }

  if (display_info) {

    for (int i = 0; i < proc_count; i++) {

      printf("\n{\n");
      printf("\tname: P%d\n", schedule[i].id);
      printf("\tarrival_time: %d\n", schedule[i].arrival_time);
      printf("\tburst_time: %d\n", schedule[i].burst_time);
      printf("\tpriority: %d\n", schedule[i].priority);
      printf("\twait_time: %d\n", schedule[i].wait_time);
      printf("\tturnaround_time: %d\n", schedule[i].turnaround_time);
      printf("}\n");
    }
  }

  if (display_sequence) {

    printf("\nExecution Order:\n");

    for (int i = 0; i < proc_count; i++) {
      printf("P%d", schedule[i].id);

      if (i != proc_count - 1)
        printf(" -> ");
    }

    printf("\n");
  }

  double total_wait = 0;
  double total_turnaround = 0;

  printf("Waiting Time:\n");
  for (int id = 1; id <= proc_count; id++) {
    for (int j = 0; j < proc_count; j++) {
      if (proc_list[j].id == id) {
        printf("P%d %d\n", proc_list[j].id, proc_list[j].wait_time);
        total_wait += proc_list[j].wait_time;
      }
    }
  }

  printf("Turnaround Time:\n");
  for (int id = 1; id <= proc_count; id++) {
    for (int j = 0; j < proc_count; j++) {
      if (proc_list[j].id == id) {
        printf("P%d %d\n", proc_list[j].id, proc_list[j].turnaround_time);
        total_turnaround += proc_list[j].turnaround_time;
      }
    }
  }

  printf("Average Waiting Time: %.2f\n", total_wait / proc_count);
  printf("Average Turnaround Time: %.2f\n",
         total_turnaround / proc_count);

  free(proc_list);
  free(schedule);

  return 0;
}
