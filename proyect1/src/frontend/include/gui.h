

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
//#include <iostream.h>
#include <time.h>

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


void updateGUI(int threadId, int progress, float piApprox, int totalProgress, float totalPiApprox);

void markCurrentThread(int threadId, gboolean unmark);

void runGUI(int argc, char **argv, int threadNum);

gboolean time_handler(GtkWidget *widget);
