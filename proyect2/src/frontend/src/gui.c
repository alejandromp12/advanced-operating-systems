
#include "include/gui.h"

bufferElementStruct buffer[];

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *bufferBox;
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


void runGUI(int argc, char *argv[], int bufferSize) {
    
    
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

	bufferBox = GTK_WIDGET(gtk_builder_get_object(builder,"BufferBox"));
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


    //grid
    //bufferGrid = GTK_WIDGET(gtk_builder_get_object(builder,"BufferGrid"));
    //bufferGrid = gtk_grid_new();

    //GtkWidget *gridContainer = GTK_WIDGET(gtk_builder_get_object(builder,"GridContainer"));
    //gtk_builder_connect_signals(builder, NULL);
    //gtk_container_add(GTK_CONTAINER (window), bufferGrid);

    gtk_label_set_text(GTK_LABEL(producerCounterLabel), "Hola");
    
    
    GtkWidget *bufferStartLabel = gtk_label_new("Buffer Start");
    gtk_container_add(GTK_CONTAINER (bufferBox), bufferStartLabel);

    //Add the buffer widgets
    for (int i=0; i<bufferSize; i++)
    {	
		//gtk_grid_insert_row(GTK_GRID(bufferGrid), i);
		//GtkWidget *dummy = gtk_label_new("Hola2");
		gtk_container_add(GTK_CONTAINER (bufferBox), buffer[i].indexLabel);

		//************************* Index Label
		char index[2];
		sprintf(index, "%d", i);
		
		buffer[i].indexLabel = gtk_label_new(index);
		//bufferGrid->insert_row(i);
		//gtk_grid_attach(GTK_GRID(bufferGrid), dummy, i, 0, 1, 1);
		//bufferGrid->attach(buffer[i].indexLabel, 0, i, 1, 1);

		//gtk_box_reorder_child(GTK_BOX(bufferBox), dummy, i);
		gtk_container_add(GTK_CONTAINER (bufferBox), buffer[i].indexLabel);
		
		// //************************* PID Label
		// char pid[2];
		// sprintf(pid, "%d", 0);
		
		// buffer[i].producerIdLabel = gtk_label_new(pid);
		// gtk_grid_attach(GTK_GRID(bufferGrid), dummy, 1, i, 1, 1);


		// //************************* Date Label
		// char date[2];
		// sprintf(date, "%d", 0);
		
		// buffer[i].dateLabel = gtk_label_new(date);
		// gtk_grid_attach(GTK_GRID(bufferGrid), dummy, 2, i, 1, 1);

		// //************************* Key Label
		// char key[2];
		// sprintf(key, "%d", 0);
		
		// buffer[i].keyLabel = gtk_label_new(key);
		// gtk_grid_attach(GTK_GRID(bufferGrid), dummy, 3, i, 1, 1);
		
	}

	GtkWidget *bufferEndLabel = gtk_label_new("Buffer End");
    gtk_container_add(GTK_CONTAINER (bufferBox), bufferEndLabel);

    gtk_label_set_text(GTK_LABEL(buffer[2].indexLabel), "Hola");
	
    
    //g_timeout_add(100, (GSourceFunc) time_handler, (gpointer) window);

    
    g_object_unref(builder);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    
}

gboolean time_handler(GtkWidget *widget) {
    
    //Sanity check
	if (widget == NULL) return FALSE;

	//printf("GUI ThreadId: %i \n", _pGuiThreadId);

	//Run Lottery
	//(*_ptrGuiLottery) ();

	//Update the GUI information
	//updateGUI(_pGuiThreadId, _pGuiThreadProgress, _pGuiPiApprox, _pGuiTotalProgress, _pGuiPiApprox);

	gtk_widget_queue_draw(widget);

	return TRUE;
}

void exit_app()
{
	gtk_main_quit();
}