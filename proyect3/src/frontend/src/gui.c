
#include "include/gui.h"
#include <gtk/gtk.h>


GtkBuilder *builder;
GtkWidget *window;

GtkWidget *myBox;
GtkWidget *titleLabel;

GtkWidget *algorithmBox;
GtkWidget *rmCheckButton;
GtkWidget *edfCheckButton;
GtkWidget *llfCheckButton;
GtkWidget *mixedSlidesCheckButton;

GtkWidget *taskQuantityBox;
GtkWidget *taskQuantityLabel;
GtkWidget *taskQuantityEntry;
GtkWidget *taskQuantityButton;

GtkWidget *taskGrid;
GtkWidget *taskColumnLabel;
GtkWidget *executionColumnLabel;
GtkWidget *periodColumnLabel;

GtkWidget *startButton;



void runGUI(int argc, char *argv[]) {
    
	_selectedRM = 0;
	_selectedEDF = 0;
	_selectedLLF = 0;
	_numTasks = 0;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "src/frontend/glade_files/gladeGUI.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder,"MyWindow"));
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	myBox = GTK_WIDGET(gtk_builder_get_object(builder,"MyBox"));
	gtk_builder_connect_signals(builder, NULL);

	titleLabel = GTK_WIDGET(gtk_builder_get_object(builder,"TitleLabel"));
	gtk_builder_connect_signals(builder, NULL);

	algorithmBox = GTK_WIDGET(gtk_builder_get_object(builder,"AlgorithmBox"));
	gtk_builder_connect_signals(builder, NULL);

	rmCheckButton = GTK_WIDGET(gtk_builder_get_object(builder,"RmCheckButton"));
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(rmCheckButton, "toggled", G_CALLBACK(rm_check), NULL);

	edfCheckButton = GTK_WIDGET(gtk_builder_get_object(builder,"EdfCheckButton"));
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(edfCheckButton, "toggled", G_CALLBACK(edf_check), NULL);

	llfCheckButton = GTK_WIDGET(gtk_builder_get_object(builder,"LlfCheckButton"));
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(llfCheckButton, "toggled", G_CALLBACK(llf_check), NULL);

	mixedSlidesCheckButton = GTK_WIDGET(gtk_builder_get_object(builder,"MixedSlidesCheckButton"));
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(mixedSlidesCheckButton, "toggled", G_CALLBACK(mixed_slides_check), NULL);

	taskQuantityBox = GTK_WIDGET(gtk_builder_get_object(builder,"TaskQuantityBox"));
	gtk_builder_connect_signals(builder, NULL);

	taskQuantityLabel = GTK_WIDGET(gtk_builder_get_object(builder,"TaskQuantityLabel"));
	gtk_builder_connect_signals(builder, NULL);

	taskQuantityEntry = GTK_WIDGET(gtk_builder_get_object(builder,"TaskQuantityEntry"));
	gtk_builder_connect_signals(builder, NULL);

	taskQuantityButton = GTK_WIDGET(gtk_builder_get_object(builder,"TaskQuantityButton"));
	gtk_builder_connect_signals(builder, NULL);
	g_signal_connect(taskQuantityButton, "clicked", G_CALLBACK(ok_clicked), NULL);

	taskGrid = GTK_WIDGET(gtk_builder_get_object(builder,"TaskGrid"));
	gtk_builder_connect_signals(builder, NULL);

	taskColumnLabel = GTK_WIDGET(gtk_builder_get_object(builder,"TaskColumnLabel"));
	gtk_builder_connect_signals(builder, NULL);

	executionColumnLabel = GTK_WIDGET(gtk_builder_get_object(builder,"ExecutionColumnLabel"));
	gtk_builder_connect_signals(builder, NULL);

	periodColumnLabel = GTK_WIDGET(gtk_builder_get_object(builder,"PeriodColumnLabel"));
	gtk_builder_connect_signals(builder, NULL);

	startButton = GTK_WIDGET(gtk_builder_get_object(builder,"StartButton"));
	g_signal_connect(startButton, "clicked", G_CALLBACK(start_clicked), NULL);
	gtk_builder_connect_signals(builder, NULL);

    
    g_object_unref(builder);
    
    gtk_widget_show_all(window);
    
    gtk_main();
    
    
}

void exit_app()
{
	gtk_main_quit();
}

void rm_check(GtkToggleButton *button)
{
	// Check whether check button was checked or unchecked
	if (gtk_toggle_button_get_active(button)) 
	{
		//printf("is Checked\n");
		_selectedRM = 1;
	}
	else 
	{
		//printf("is Unchecked\n");
		_selectedRM = 0;
	}
}

void edf_check(GtkToggleButton *button)
{
	// Check whether check button was checked or unchecked
	if (gtk_toggle_button_get_active(button)) 
	{
		_selectedEDF = 1;
	}
	else 
	{
		//printf("is Unchecked\n");
		_selectedEDF = 0;
	}
}

void llf_check(GtkToggleButton *button)
{
	// Check whether check button was checked or unchecked
	if (gtk_toggle_button_get_active(button)) 
	{
		_selectedLLF = 1;
	}
	else 
	{
		_selectedLLF = 0;
	}
}

void mixed_slides_check(GtkToggleButton *button)
{
	// Check whether check button was checked or unchecked
	if (gtk_toggle_button_get_active(button)) 
	{
		_mixedSlides = 1;
	}
	else 
	{
		_mixedSlides = 0;
	}
}

void ok_clicked(GtkButton *button)
{
	const gchar *entrychar = gtk_entry_get_text(GTK_ENTRY(taskQuantityEntry));
	int number = atoi(entrychar);
	if(number>0 && number<7)
	{
		_numTasks = number;

		//call populate method
		populateGrid();

		//block OK button
		gtk_widget_set_sensitive (taskQuantityButton, FALSE);
	}
	else
	{
		printf("Error, input must be a number between 1 and 6\n");
	}
}

void populateGrid()
{
	char tmp[100];
	for(int i = 0 ; i < _numTasks; i++)
	{	
		sprintf(tmp, "%i \n", i+1);
		tasksGUI[i].taskIdLabel = gtk_label_new(tmp);

		tasksGUI[i].executionEntry = gtk_entry_new();

		tasksGUI[i].periodEntry = gtk_entry_new();
	}

	for(int i = 0 ; i < _numTasks; i++)
	{	
		gtk_grid_insert_row(GTK_GRID(taskGrid), i+1);
		gtk_grid_attach(GTK_GRID(taskGrid), tasksGUI[i].taskIdLabel, 0, i+1, 1 ,1);

		gtk_grid_attach(GTK_GRID(taskGrid), tasksGUI[i].executionEntry, 1, i+1, 1 ,1);

		gtk_grid_attach(GTK_GRID(taskGrid), tasksGUI[i].periodEntry, 2, i+1, 1 ,1);
	}

	gtk_widget_queue_draw(window);
	gtk_widget_show_all(window);
}


void start_clicked(GtkButton *button)
{
	for(int i = 0 ; i < _numTasks; i++)
	{	
		const gchar *entrychar = gtk_entry_get_text(GTK_ENTRY(tasksGUI[i].executionEntry));
		int executionTime = atoi(entrychar);

		entrychar = gtk_entry_get_text(GTK_ENTRY(tasksGUI[i].periodEntry));
		int periodTime = atoi(entrychar);

		//printf("Execution time %d, Period Time %d \n", executionTime, periodTime);

		if(executionTime>0 && periodTime>0 && executionTime <= periodTime)
		{
			tasksGUI[i].executionTime = executionTime;
			tasksGUI[i].periodTime = periodTime;

			//callback
			(*_ptrGetFromGUI) ();
		}
		else
		{
			printf("Error, one or more task times are invalid.");
			return;
		}
	}
}