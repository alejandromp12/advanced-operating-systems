

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


void runGUI(int argc, char **argv);

//gboolean time_handler(GtkWidget *widget);

void exit_app();

void rm_check(GtkToggleButton *button);
void edf_check(GtkToggleButton *button);
void llf_check(GtkToggleButton *button);
void ok_clicked(GtkButton *button);
void start_clicked(GtkButton *button);
void populateGrid();


//Pointers to update the GUI
// int _pGuiThreadId;
// int _pGuiThreadProgress;
// int _pGuiTotalProgress;
// float _pGuiPiApprox;

void (*_ptrGetFromGUI)();

#endif
