#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
//#include <iostream.h>
#include <time.h>

#define NUM_THREADS 10

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

threadStruct threadBank[];
int _totalProgress = 0;

static GtkWidget *window;
static GtkWidget *grid;
static GtkWidget *totalProgressLabel;
static GtkWidget *totalProgressBar;
static GtkWidget *totalPercentageLabel;
static GtkWidget *totalPiApproxLabel;


void update_UI(int threadId, int progress, float piApprox)
{
	int i = threadId - 1;
	threadBank[i].progress = progress;
	threadBank[i].piApprox = piApprox;
	
	//************************* Thread Pi Approximate
		
	char piApproxString[10];
	sprintf(piApproxString, "%f", threadBank[i].piApprox);
	char piApproxText[] = "Pi: ";
	strncat(piApproxText, piApproxString, 10);
	//threadBank[i].piApproxLabel = gtk_label_new(piApproxText);
	gtk_label_set_text(GTK_LABEL(threadBank[i].piApproxLabel), piApproxText);
	//gtk_grid_attach(GTK_GRID(grid), threadBank[i].piApproxLabel, 1, rowCount, 1, 1);
	//rowCount++;
		
	//************************* Thread Progress Label
	char percentageString[4];
	sprintf(percentageString, "%d", threadBank[i].progress);
	strncat(percentageString, "\%", 1);
		
	//threadBank[i].progressLabel = gtk_label_new(percentageString);
	gtk_label_set_text(GTK_LABEL(threadBank[i].progressLabel), percentageString);
	//gtk_grid_attach(GTK_GRID(grid), threadBank[i].progressLabel, 1, rowCount, 1, 1);
		
		
	//************************* Thread Progress Bar
		
	float percentageFloat = (float)threadBank[i].progress / 100;
	//threadBank[i].progressBar = gtk_progress_bar_new ();
	//gtk_grid_attach(GTK_GRID(grid), threadBank[i].progressBar, 0, rowCount, 1, 1);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (threadBank[i].progressBar), percentageFloat);
	//rowCount++;
}


int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

	//GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	//GtkWidget *grid;
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    //****** Total Progress Label
    //GtkWidget *totalProgressLabel;
    totalProgressLabel = gtk_label_new("Total Progress");
    gtk_grid_attach(GTK_GRID(grid), totalProgressLabel, 0, 0, 1, 1);
    
    //****** Total Progress Bar
    //GtkWidget *totalProgressBar;
    totalProgressBar = gtk_progress_bar_new ();
    gtk_grid_attach(GTK_GRID(grid), totalProgressBar, 0, 1, 1, 1);
    
    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (totalProgressBar), 0.5);
    
    //GdkColor color;
    //gdk_color_parse("blue",&color);
    //gtk_widget_modify_bg(totalProgressBar,GTK_STATE_ACTIVE, GdkRGBA(0.0, 1.0, 0.0, 1.0));
    
    //****** Total Percentage
    int totalPercentageNum = 0;
    char totalPercentageString[4];
	sprintf(totalPercentageString, "%d", totalPercentageNum);
	strncat(totalPercentageString, "\%", 1);
    //GtkWidget *totalPercentageLabel;
    totalPercentageLabel = gtk_label_new(totalPercentageString);
    gtk_grid_attach(GTK_GRID(grid), totalPercentageLabel, 1, 1, 1, 1);
    
    //****** Total Pi Approximate
    float totalPiApproxNum = 3.1415;
    char totalPiApproxString[10];
    sprintf(totalPiApproxString, "%f", totalPiApproxNum);
    char totalPiApproxText[] = "Pi: ";
    strncat(totalPiApproxText, totalPiApproxString, 10);
    //GtkWidget *totalPiApproxLabel;
    totalPiApproxLabel = gtk_label_new(totalPiApproxText);
    gtk_grid_attach(GTK_GRID(grid), totalPiApproxLabel, 1, 0, 1, 1);
    
    
    int rowCount = 2;
    
    //threadBank[NUM_THREADS];
    
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
		
		gtk_grid_attach(GTK_GRID(grid), threadBank[i].threadIdLabel, 0, rowCount, 1, 1);
		
		
		//************************* Thread Pi Approximate
		
		char piApproxString[10];
		sprintf(piApproxString, "%f", threadBank[i].piApprox);
		char piApproxText[] = "Pi: ";
		strncat(piApproxText, piApproxString, 10);
		threadBank[i].piApproxLabel = gtk_label_new(piApproxText);
		gtk_grid_attach(GTK_GRID(grid), threadBank[i].piApproxLabel, 1, rowCount, 1, 1);
		rowCount++;
		
		//************************* Thread Progress Label
		char percentageString[4];
		sprintf(percentageString, "%d", threadBank[i].progress);
		strncat(percentageString, "\%", 1);
		
		threadBank[i].progressLabel = gtk_label_new(percentageString);
		gtk_grid_attach(GTK_GRID(grid), threadBank[i].progressLabel, 1, rowCount, 1, 1);
		
		
		//************************* Thread Progress Bar
		
		float percentageFloat = (float)threadBank[i].progress / 100;
		threadBank[i].progressBar = gtk_progress_bar_new ();
		gtk_grid_attach(GTK_GRID(grid), threadBank[i].progressBar, 0, rowCount, 1, 1);
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (threadBank[i].progressBar), percentageFloat);
		rowCount++;
		
	}
	
    update_UI(7, 65, 3.1415);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    

    return 0;
}
