
#include "include/gui.h"

threadStruct threadBank[] = {};
int _previousThread = 0;

static GtkWidget *window;
static GtkWidget *grid;
static GtkWidget *totalProgressLabel;
static GtkWidget *totalProgressBar;
static GtkWidget *totalPercentageLabel;
static GtkWidget *totalPiApproxLabel;

void setPercentageLabel(int progress, GtkWidget *label)
{
	char percentageString[4];
	sprintf(percentageString, "%d", progress);
	strncat(percentageString, "\%", 1);
	gtk_label_set_text(GTK_LABEL(label), percentageString);
}

void setProgressBar(int progress, GtkWidget *bar)
{
	float percentageFloat = (float)progress / 100;
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (bar), percentageFloat);
}

void setPiApproxLabel(float piApprox, GtkWidget *label)
{
	char piApproxString[10];
	sprintf(piApproxString, "%f", piApprox);
	char piApproxText[] = "Pi: ";
	strncat(piApproxText, piApproxString, 10);
	gtk_label_set_text(GTK_LABEL(label), piApproxText);
}

void updateGUI(int threadId, int progress, float piApprox, int totalProgress, float totalPiApprox)
{
	int i = threadId - 1;
	threadBank[i].progress = progress;
	threadBank[i].piApprox = piApprox;
	
	// Thread Pi Approximate
	setPiApproxLabel(threadBank[i].piApprox, threadBank[i].piApproxLabel);
		
	// Thread Percentage Label
	setPercentageLabel(threadBank[i].progress, threadBank[i].progressLabel);
			
	// Thread Progress Bar
	setProgressBar(threadBank[i].progress, threadBank[i].progressBar);
		
	// Total Percentage Label
	setPercentageLabel(totalProgress, totalPercentageLabel);

	// Total Progress Bar
	setProgressBar(totalProgress, totalProgressBar);

	// Total Pi Approimate
	setPiApproxLabel(totalPiApprox, totalPiApproxLabel);
}

void markCurrentThread(int threadId, gboolean unmark)
{
	char id[2];
	sprintf(id, "%d", threadId);
	char labelId[] = "Thread ";
	strncat(labelId, id, 2);

	char *format = "<span foreground=\"#00dd00\">\%s</span>";
	if(unmark)
	{
		format = "<span foreground=\"#000000\">\%s</span>";
	}
	
	char *markup;
	
	markup = g_markup_printf_escaped (format, labelId);
	gtk_label_set_markup (GTK_LABEL (threadBank[threadId-1].threadIdLabel), markup);
	g_free (markup);
}


void runGUI(int argc, char **argv, int threadNum) {
    
    gtk_init(&argc, &argv);

	//Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


	//Grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    

    //Total Progress Label
    totalProgressLabel = gtk_label_new("Total Progress");
    gtk_grid_attach(GTK_GRID(grid), totalProgressLabel, 0, 0, 1, 1);
    

    //Total Progress Bar
    totalProgressBar = gtk_progress_bar_new ();
    gtk_grid_attach(GTK_GRID(grid), totalProgressBar, 0, 1, 1, 1);
    gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (totalProgressBar), 0);
    
    
    //Total Percentage
    totalPercentageLabel = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), totalPercentageLabel, 1, 1, 1, 1);
    setPercentageLabel(0, totalPercentageLabel);
    

    //Total Pi Approximate
    totalPiApproxLabel = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), totalPiApproxLabel, 1, 0, 1, 1);
    setPiApproxLabel(0, totalPiApproxLabel);
    
    
    //Add the thread widgets
    int rowCount = 2;
    for (int i=0; i<threadNum; i++)
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
	
    
    g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) window);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    
}

gboolean time_handler(GtkWidget *widget) {
    
    //Sanity check
	if (widget == NULL) return FALSE;

	//printf("GUI ThreadId: %i \n", _pGuiThreadId);

	//Run Lottery
	(*_ptrGuiLottery) ();

	//Update the GUI information
	updateGUI(_pGuiThreadId, _pGuiThreadProgress, _pGuiPiApprox, _pGuiTotalProgress, _pGuiPiApprox);

	//Unmark the previous running thread
	if(_previousThread != 0)
	{
		markCurrentThread(_previousThread, TRUE);
	}

	//Mark the current thread label in green
	markCurrentThread(_pGuiThreadId, FALSE);

	//Register as previous thread for the next cycle
	_previousThread = _pGuiThreadId;

	gtk_widget_queue_draw(widget);

	return TRUE;
}
