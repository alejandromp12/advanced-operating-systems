#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
//#include <iostream.h>

#define NUM_THREADS 10

static GtkWidget *number1;
static GtkWidget *number2;
static GtkWidget *result;

int _totalProgress = 0;

/*
void do_calculate(GtkWidget *calculate, gpointer data) {
    int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "result: %d", num1 + num2);

    gtk_label_set_text(GTK_LABEL(result), buffer);
} */

typedef struct
{
	int threadId;
	int progress;
	float piApprox;
	GtkWidget *threadIdLabel;
	GtkWidget *progressLabel;
	GtkWidget *progressBar;
	GtkWidget *piApproxLabel;
	
} threadStruct; ///< Struct to handle threads information


// gcc 007_gtk.c -o 007_gtk `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

	GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *grid;
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    GtkWidget *totalProgressLabel;
    totalProgressLabel = gtk_label_new("Total Progress");
    gtk_grid_attach(GTK_GRID(grid), totalProgressLabel, 0, 0, 1, 1);
    
    GtkWidget *totalProgressBar;
    totalProgressBar = gtk_progress_bar_new ();
    gtk_grid_attach(GTK_GRID(grid), totalProgressBar, 0, 1, 1, 1);
    
    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (totalProgressBar), 0.5);
    
    GtkWidget *totalPercentage;
    totalPercentage = gtk_label_new(" 0 \%");
    gtk_grid_attach(GTK_GRID(grid), totalPercentage, 2, 0, 1, 1);
    
    int rowCount = 2;
    
    threadStruct threadBank[NUM_THREADS];
    
    for (int i=0; i<NUM_THREADS; i++)
    {
		threadBank[i].threadId = i + 1;
		threadBank[i].progress = 50;
		threadBank[i].piApprox = 3.14;
		
		
		//************************* Thread Id Label
		char id[2];
		sprintf(id, "%d", i+1);
		char labelId[] = "Thread ";
		strncat(labelId, id, 2);
		
		threadBank[i].threadIdLabel = gtk_label_new(labelId);
		
		rowCount++;
		gtk_grid_attach(GTK_GRID(grid), threadBank[i].threadIdLabel, 0, rowCount, 1, 1);
		
		//************************* Thread Progress Label
		char percentage[3];
		sprintf(percentage, "%d", threadBank[i].progress);
		char labelPercentage[] = "Progress ";
		strncat(labelPercentage, percentage, 3);
		strncat(labelPercentage, "\%", 1);
		
		threadBank[i].progressLabel = gtk_label_new(labelPercentage);
		gtk_grid_attach(GTK_GRID(grid), threadBank[i].progressLabel, 1, rowCount, 1, 1);
		rowCount++;
		
		//************************* Thread Progress Bar
		
		float percentageFloat = (float)threadBank[i].progress / 100;
		threadBank[i].progressBar = gtk_progress_bar_new ();
		gtk_grid_attach(GTK_GRID(grid), threadBank[i].progressBar, 0, rowCount, 1, 1);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (threadBank[i].progressBar), percentageFloat);
		rowCount++;
		
		
		
	}
    

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
