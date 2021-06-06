

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
#include <time.h>


#ifndef GUI_FILE
#define GUI_FILE


#define MAX_TASKS_GUI 6

typedef struct
{
	GtkWidget *taskIdLabel;
	GtkWidget *executionEntry;
	GtkWidget *periodEntry;
	int executionTime;
	int periodTime;
	
} taskGuiStruct; ///< Struct to handle threads information


taskGuiStruct tasksGUI[MAX_TASKS_GUI];
int _selectedRM;
int _selectedEDF;
int _selectedLLF;

int _numTasks;

int _mixedSlides;


void runGUI(int argc, char **argv);


void exit_app();

void rm_check(GtkToggleButton *button);
void edf_check(GtkToggleButton *button);
void llf_check(GtkToggleButton *button);
void mixed_slides_check(GtkToggleButton *button);
void ok_clicked(GtkButton *button);
void start_clicked(GtkButton *button);
void populateGrid();

void (*_ptrGetFromGUI)();

#endif
