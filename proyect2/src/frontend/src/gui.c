
#include "include/gui.h"
#include <gtk/gtk.h>


GtkBuilder *builder;
GtkWidget *window;

GtkWidget *bufferScrollWindowGrid;
GtkWidget *bufferViewGrid;
GtkWidget *bufferGrid;
GtkWidget *generalBox;
GtkWidget *counterBox;
GtkWidget *logBox;
GtkWidget *logScrollWindow;
GtkWidget *logViewport;

GtkWidget *titleLabel;
GtkWidget *producerCounterLabel;
GtkWidget *consumerCounterLabel;

// static GtkWidget *totalProgressLabel;
// static GtkWidget *totalProgressBar;
// static GtkWidget *totalPercentageLabel;
// static GtkWidget *totalPiApproxLabel;

// void setPercentageLabel(int progress, GtkWidget *label)
// {
// 	char percentageString[4];
// 	sprintf(percentageString, "%d", progress);
// 	strncat(percentageString, "\%", 1);
// 	gtk_label_set_text(GTK_LABEL(label), percentageString);
// }

// void setProgressBar(int progress, GtkWidget *bar)
// {
// 	float percentageFloat = (float)progress / 100;
// 	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (bar), percentageFloat);
// }

// void setPiApproxLabel(float piApprox, GtkWidget *label)
// {
// 	char piApproxString[10];
// 	sprintf(piApproxString, "%f", piApprox);
// 	char piApproxText[] = "Pi: ";
// 	strncat(piApproxText, piApproxString, 10);
// 	gtk_label_set_text(GTK_LABEL(label), piApproxText);
// }

// void updateGUI(int threadId, int progress, float piApprox, int totalProgress, float totalPiApprox)
// {
// 	int i = threadId - 1;
// 	threadBank[i].progress = progress;
// 	threadBank[i].piApprox = piApprox;
	
// 	// Thread Pi Approximate
// 	setPiApproxLabel(threadBank[i].piApprox, threadBank[i].piApproxLabel);
		
// 	// Thread Percentage Label
// 	setPercentageLabel(threadBank[i].progress, threadBank[i].progressLabel);
			
// 	// Thread Progress Bar
// 	setProgressBar(threadBank[i].progress, threadBank[i].progressBar);
		
// 	// Total Percentage Label
// 	setPercentageLabel(totalProgress, totalPercentageLabel);

// 	// Total Progress Bar
// 	setProgressBar(totalProgress, totalProgressBar);

// 	// Total Pi Approimate
// 	setPiApproxLabel(totalPiApprox, totalPiApproxLabel);
// }


void runGUI(int argc, char *argv[], int bufferId) {
    

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "src/frontend/glade_files/gladeGUI.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder,"MyWindow"));
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	generalBox = GTK_WIDGET(gtk_builder_get_object(builder,"MyBox"));
	gtk_builder_connect_signals(builder, NULL);

	counterBox = GTK_WIDGET(gtk_builder_get_object(builder,"CounterBox"));
	gtk_builder_connect_signals(builder, NULL);


	bufferScrollWindowGrid = GTK_WIDGET(gtk_builder_get_object(builder,"BufferScrollWindowGrid"));
	gtk_builder_connect_signals(builder, NULL);

	bufferViewGrid = GTK_WIDGET(gtk_builder_get_object(builder,"BufferViewGrid"));
	gtk_builder_connect_signals(builder, NULL);

	bufferGrid = GTK_WIDGET(gtk_builder_get_object(builder,"BufferGrid"));
	gtk_builder_connect_signals(builder, NULL);

	logBox = GTK_WIDGET(gtk_builder_get_object(builder,"LogBox"));
	gtk_builder_connect_signals(builder, NULL);


	logScrollWindow = GTK_WIDGET(gtk_builder_get_object(builder,"LogScrollWindow"));
	gtk_builder_connect_signals(builder, NULL);


	logViewport = GTK_WIDGET(gtk_builder_get_object(builder,"LogViewport"));
	gtk_builder_connect_signals(builder, NULL);


	titleLabel = GTK_WIDGET(gtk_builder_get_object(builder,"TitleLabel"));
	gtk_builder_connect_signals(builder, NULL);


	producerCounterLabel = GTK_WIDGET(gtk_builder_get_object(builder,"ProducerCounter"));
	gtk_builder_connect_signals(builder, NULL);


	consumerCounterLabel = GTK_WIDGET(gtk_builder_get_object(builder,"ConsumerCounter"));
	gtk_builder_connect_signals(builder, NULL);

    //Add the buffer widgets
	char tmp[100];

	//Change title to indicate Buffer ID
	sprintf(tmp, "Buffer %i \n", bufferId);
	gtk_label_set_text(GTK_LABEL(titleLabel),tmp);


	for(int i = 0 ; i < _bufferSizeGUI; i++)
	{	
		sprintf(tmp, "%i \n", i+1);
		bufferGUI[i].indexLabel = gtk_label_new(tmp);

		sprintf(tmp, "%i \n", 0);
		bufferGUI[i].producerIdLabel = gtk_label_new(tmp);

		sprintf(tmp, "%i \n", 0);
		bufferGUI[i].dateLabel = gtk_label_new(tmp);

		sprintf(tmp, "%i \n", 0);
		bufferGUI[i].keyLabel = gtk_label_new(tmp);
	}

	GtkWidget *indexLabel = gtk_label_new("Index");
	GtkWidget *pidLabel = gtk_label_new("PID");
	GtkWidget *dateLabel = gtk_label_new("Date");
	GtkWidget *keyLabel = gtk_label_new("Key");

	gtk_grid_insert_column(GTK_GRID(bufferGrid), 0);
	gtk_grid_attach(GTK_GRID(bufferGrid), indexLabel, 0, 0, 1 ,1);
	gtk_grid_attach(GTK_GRID(bufferGrid), pidLabel, 0, 1, 1 ,1);
	gtk_grid_attach(GTK_GRID(bufferGrid), dateLabel, 0, 2, 1 ,1);
	gtk_grid_attach(GTK_GRID(bufferGrid), keyLabel, 0, 3, 1 ,1);



	for(int i = 0 ; i < _bufferSizeGUI; i++)
	{	
		gtk_grid_insert_column(GTK_GRID(bufferGrid), i+1);
		gtk_grid_attach(GTK_GRID(bufferGrid), bufferGUI[i].indexLabel, i+1, 0, 1 ,1);

		gtk_grid_attach(GTK_GRID(bufferGrid), bufferGUI[i].producerIdLabel, i+1, 1, 1 ,1);

		gtk_grid_attach(GTK_GRID(bufferGrid), bufferGUI[i].dateLabel, i+1, 2, 1 ,1);

		gtk_grid_attach(GTK_GRID(bufferGrid), bufferGUI[i].keyLabel, i+1, 3, 1 ,1);
	}
			
		

	/* GtkWidget *tmp = gtk_label_new("Holaaaa");

	gtk_grid_insert_column(GTK_GRID(bufferGrid), 1);
	gtk_grid_attach(GTK_GRID(bufferGrid), tmp, 0, 1, 1 ,1);

	GtkWidget *tmp2 = gtk_label_new("Holaaaa");

	gtk_grid_insert_column(GTK_GRID(bufferGrid), 2);
	gtk_grid_attach(GTK_GRID(bufferGrid), tmp, 0, 2, 1 ,1); */
		
	


	
    
    g_timeout_add(1, (GSourceFunc) time_handler, (gpointer) window);

    
    g_object_unref(builder);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    
}

gboolean time_handler(GtkWidget *widget) {
    
    //Sanity check
	if (widget == NULL) return FALSE;

	//printf("GUI ThreadId: %i \n", _pGuiThreadId);

	//Run Creator
	(*_ptrUpdateGUI) ();

	//Update the GUI information
	//updateGUI(_pGuiThreadId, _pGuiThreadProgress, _pGuiPiApprox, _pGuiTotalProgress, _pGuiPiApprox);

	gtk_widget_queue_draw(widget);

	return TRUE;
}

void exit_app()
{
	gtk_main_quit();
}
