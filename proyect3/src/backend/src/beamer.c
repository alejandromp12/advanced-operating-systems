#include "include/beamer.h"
#include <math.h>


void InitPresentation()
{
    OUTPUT = fopen("presentation/outputBeamer.tex","w");
    setCover();
}


void setCover()
{
    fprintf(OUTPUT, "%s", "\\documentclass[xcolor=table]{beamer}\n");
    fprintf(OUTPUT, "%s", "\\usetheme{Madrid}\n");
    fprintf(OUTPUT, "%s", "\\usecolortheme{default}\n");
    fprintf(OUTPUT, "%s", "\\usepackage{blkarray, bigstrut}");
    fprintf(OUTPUT, "%s", "\\usepackage{tikz}");
    fprintf(OUTPUT, "%s", "%Information to be included in the title page:\n");
    fprintf(OUTPUT, "%s", "\\title[Real-Time Scheduling] %optional\n");
    fprintf(OUTPUT, "%s", "{Project 3 - Real Time Scheduling}\n");
    fprintf(OUTPUT, "%s", "\\subtitle{RM - EDF - LLF}\n");
    fprintf(OUTPUT, "%s", "\\author[Alejandro , Javier, Izcar] % (optional, for multiple authors)\n");
    fprintf(OUTPUT, "%s", "{~Alejandro Mora\\inst{1} \\and ~Javier Cordero\\inst{1} \\and ~Izcar Muñoz\\inst{1}}\n");
    fprintf(OUTPUT, "%s", "\\institute[ITCR] % (optional)\n");
    fprintf(OUTPUT, "%s", "{\n");
    fprintf(OUTPUT, "%s", "\\inst{1}%\n");
    fprintf(OUTPUT, "%s", "Maestría en Computación\\\\\n");
    fprintf(OUTPUT, "%s", "Costa Rica Institute of Technology\n");
    fprintf(OUTPUT, "%s", "}\n");
    fprintf(OUTPUT, "%s", "\\date[SOA 2021] % (optional)\n");
    fprintf(OUTPUT, "%s", "{Advanced Operating Systems, June 2021}\n");

}

void startPresentation()
{
    fprintf(OUTPUT, "%s", "\\begin{document}\n");
    fprintf(OUTPUT, "%s", "\\frame{\\titlepage}\n");
}

void finistPresentation()
{

    fprintf(OUTPUT, "%s", "\\end{document}\n");

    fclose(OUTPUT);
}

void setFrame(char *title, char content[])
{
    fprintf(OUTPUT, "%s", "\\begin{frame}[shrink=20]\n");
    fprintf(OUTPUT, "\\frametitle{%s}\n", title);
    fprintf(OUTPUT, "%s\n", content);
    
}

void definitionRM()
{      
    char *title = "RM: Rate Monothonic";
    char *content = "Rate monotonic (RM)  scheduling is a priority algorithm that belongs to the static priority scheduling category of Real Time Operating Systems. It is preemptive in nature. The priority is decided according to the cycle time of the processes that are involved. If the process has a small job duration, then it has the highest priority. Thus if a process with highest priority starts execution, it will preempt the other running processes. The priority of a process is inversely proportional to the period it will run for.A set of processes can be scheduled by the RM only if they satisfy the following equation :";
    setFrame(title, content);
    fprintf(OUTPUT, "%s", "\\begin{itemize} \\item Schedulability Test \\newline");
    fprintf(OUTPUT, "%s", "\\[\\sum_{i=1}^{n }C_i/T_i  \\leq  n(2^{1/n} - 1)\\] \\[\\lim_{n\\to \\infty}  n(2^{1/n} - 1) = ln(2) \\simeq 0.69\\]");
    fprintf(OUTPUT, "%s", "\\newline where $n$ is the number of processes");
    fprintf(OUTPUT, "%s", "\\newline  $C_{i}$ is the computation time of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\newline  $T_{i}$ is the period of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\end{itemize}");
    fprintf(OUTPUT, "%s", "\\end{frame}\n");
}

void definitionEDF()
{
    char *title = "EDF: Earliest Deadline First";
    char *content = " Dynamic scheduling algorithm that works with dynamic priorities, these are inversely proportional to the remaining time to the deadline. Every time a new task reaches the ready queue, priorities are recalculated and could task away the CPU from the current task.";
    setFrame(title, content);
    fprintf(OUTPUT, "%s", "\\begin{itemize} \\item Schedulability Test \\newline");
    fprintf(OUTPUT, "%s", "\\[\\sum_{i=1}^{n }C_i/T_i \\; \\leq  1\\] \\newline where $n$ is the number of processes");
    fprintf(OUTPUT, "%s", "\\newline  $C_{i}$ is the computation time of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\newline  $T_{i}$ is the period of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\end{itemize}");
    fprintf(OUTPUT, "%s", "\\end{frame}\n");
}

void definitionLLF()
{   
    char *title = "LLF: Least Laxity First";
    char *content = " Dynamic scheduling algorithm that works with dynamic priorities, these are inversely proportional to the task's laxity, which is defined as the remaining time to deadline minus the remaining computation time. Every time a new task reaches the ready queue, priorities are recalculated and could take away the CPU from the current task.";
    setFrame(title, content);
    fprintf(OUTPUT, "%s", "\\begin{itemize} \\item Schedulability Test \\newline");
    fprintf(OUTPUT, "%s", "\\[\\sum_{i=1}^{n }C_i/T_i \\; \\leq  1\\]");
    fprintf(OUTPUT, "%s", "\\newline where $n$ is the number of processes");
    fprintf(OUTPUT, "%s", "\\newline  $C_{i}$ is the computation time of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\newline  $T_{i}$ is the period of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\end{itemize}");
    fprintf(OUTPUT, "%s", "\\end{frame}\n");
}

void RMSchedulabilityTest(int executionTime[], int deadline[], int numProcesses)
{

    setFrame("Schedulability Test", "");


    fprintf(OUTPUT, "%s", "\\begin{itemize}");
    fprintf(OUTPUT, "%s", "\\item RM Schedulability Test \\newline");
    fprintf(OUTPUT, "%s", "\\[\\sum_{i=1}^{n }C_i/T_i  \\leq  n(2^{1/n} - 1)\\]");

    fprintf(OUTPUT, "%s", "\\[");
    double result = 0;
    double condition = (double)numProcesses * (pow(2.0, (1.0/(double)numProcesses )) - 1);
    for(int i = 0; i < numProcesses; i ++)
    {
        fprintf(OUTPUT, "%d/%d %s", executionTime[i], deadline[i], i == (numProcesses - 1)? "= ": "+");
        result += (double)executionTime[i]/ (double)deadline[i];
    }
     fprintf(OUTPUT, " %f %s", result,"\\]");

    fprintf(OUTPUT, "\\[n (2^{1/n} - 1) = %d (2^{1/%d} - 1) = %f\\]\\newline",numProcesses, numProcesses,condition);
    if(result <=condition)
    {
        fprintf(OUTPUT, "%s", " Problem is schedulable for RM test");
    }
    else{
        fprintf(OUTPUT, "%s", " Problem is not schedulable for RM test");
    }
    fprintf(OUTPUT, "%s", "\\newline where $n$ is the number of processes");
    fprintf(OUTPUT, "%s", "\\newline  $C_{i}$ is the computation time of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\newline  $T_{i}$ is the period of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\end{itemize}");
    
    fprintf(OUTPUT, "%s", "\\end{frame}\n");
}

void EDFSchedulabilityTest(int executionTime[], int deadline[], int numProcesses)
{

    setFrame("Schedulability Test", "");

    fprintf(OUTPUT, "%s", "\\begin{itemize}");
    fprintf(OUTPUT, "%s", "\\item EDF Schedulability Test \\newline");
    fprintf(OUTPUT, "%s", "\\[\\sum_{i=1}^{n }C_i/T_i \\; \\leq  1\\] ");

    fprintf(OUTPUT, "%s", "\\[");
    double result = 0;
    double condition = 1;
    for(int i = 0; i < numProcesses; i ++)
    {
        fprintf(OUTPUT, "%d/%d %s", executionTime[i], deadline[i], i == (numProcesses - 1)? "= ": "+");
        result += (double)executionTime[i]/ (double)deadline[i];
    }
     fprintf(OUTPUT, " %f %s", result,"\\]");

    fprintf(OUTPUT, "\\[%f %s %f\\]",result, result <= condition? "\\leq": ">",condition);
    if(result <=condition)
    {
        fprintf(OUTPUT, "%s", " Problem is schedulable for EDF test");
    }
    else{
        fprintf(OUTPUT, "%s", " Problem is not schedulable EDF RM test");
    }
    fprintf(OUTPUT, "%s", "\\newline where $n$ is the number of processes");
    fprintf(OUTPUT, "%s", "\\newline  $C_{i}$ is the computation time of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\newline  $T_{i}$ is the period of $process_{i}$");
    fprintf(OUTPUT, "%s", "\\end{itemize}");
    fprintf(OUTPUT, "%s", "\\end{frame}\n");
}




void SchedulabilityTest(int executionTime[], int deadline[], int numProcesses)
{
    RMSchedulabilityTest(executionTime, deadline, numProcesses);
    EDFSchedulabilityTest(executionTime, deadline, numProcesses);
}
void simulationStep(char *title, int processList[], int cycles, int numProcesses, int deadline[], int deadProcess, int t_deadProcess, int executionTime[])
{  
    int step_tmp ;
    int quit = 0;
    for(int step = 1; step <= cycles + 1; step ++)
    {
        step_tmp = step;
        fprintf(OUTPUT, "%s","\\begin{frame}"); 
        fprintf(OUTPUT, "\\frametitle{%s - Time %d}", title, step);
        fprintf(OUTPUT, "\\scalebox{0.7}{\\begin{tabular}{");
        for(int i = 0; i <= cycles + 1; i++)
        {
            fprintf(OUTPUT, "|c");
        }
        fprintf(OUTPUT, "|}\n \\multicolumn{1}{l}{} &" );
        for(int i = 1; i <= cycles ; i++)
        {
            fprintf(OUTPUT, "\\multicolumn{1}{l}{\\fontsize{2.5}{4}\\selectfont{%d}} %s", i , i < cycles? "&": "\\\\");
        }


        for(int i  = 0; i < numProcesses; i++)
        {
            fprintf(OUTPUT, "\\hline \n");
            fprintf(OUTPUT, "\\tiny Task %d &", i  +1);
            for(int j = 1; j < cycles + 1; j++)
            {   
                if(j <= step)
                {
                    if(processList[j - 1] == i + 1)
                    {
                        if((j) % deadline[i] == 0)
                        {
                            if(deadProcess == (i  + 1) && j == t_deadProcess + 1)
                            {
                                fprintf(OUTPUT, "\\multicolumn{1}{%sc!{\\color{red}\\vrule width 2pt}}{\\cellcolor{blue!25}}", j == 1 ? "|": "");

                            }
                            else{

                                 fprintf(OUTPUT, "\\multicolumn{1}{%sc!{\\color{blue}\\vrule width 1pt}}{\\cellcolor{blue!25}}", j == 1? "|": "");
                            }
                        }
                        else{
                            int k;
                            int tmp_j = j;
                            for(k =1; k <= executionTime[i]; k++)
                            {   tmp_j += 1;
                                if(!(processList[tmp_j - 1] == i + 1)) break;
                            }
                            j = tmp_j -1;
                            step_tmp = j > step_tmp? j: step_tmp;

                            if((j) % deadline[i] == 0)
                            {
                                if(deadProcess == (i  + 1) && j == t_deadProcess + 1)
                                {
                                    fprintf(OUTPUT, "\\multicolumn{%d}{c!{\\color{red}\\vrule width 2pt}}{\\cellcolor{blue!25}}", k);
                                }
                                else
                                {
                                    fprintf(OUTPUT, "\\multicolumn{%d}{c!{\\color{blue}\\vrule width 1pt}}{\\cellcolor{blue!25}}", k);
                                }
                                
                            }
                            else{

                                fprintf(OUTPUT, "\\multicolumn{%d}{c|}{\\cellcolor{blue!25}}", k);
                            }
                        }
                    }
                    else
                    {
                        if(j% deadline[i] == 0)
                        {  
                            if(deadProcess == (i  + 1) && j == t_deadProcess + 1)
                            {
                                fprintf(OUTPUT, "\\multicolumn{1}{%sc!{\\color{red}\\vrule width 2pt}}{}", j == 1 ? "|": "");
                            }
                            else{

                                fprintf(OUTPUT, "\\multicolumn{1}{c!{\\color{blue}\\vrule width 1pt}}{}");
                            }
                            
                        }
                        else{

                            fprintf(OUTPUT, " ");
                        }
                    }
                }
                else{

                    if(j % deadline[i] == 0)
                    {
                        fprintf(OUTPUT, "\\multicolumn{1}{c!{\\color{blue}\\vrule width 1pt}}{}");   
                    }
                    else{
                        fprintf(OUTPUT, " ");
                    }
                }
                if(!(j == cycles))
                {
                    fprintf(OUTPUT, "&");
                }
            }
            fprintf(OUTPUT, "\\\\\n");
            fprintf(OUTPUT, "\\hline \n");
        }
        
        fprintf(OUTPUT, "\\end{tabular}}");
        fprintf(OUTPUT, "%s","\\end{frame}");

        step = step_tmp;


        if(quit == 1)
            break;

        if(step == t_deadProcess + 1)
        {
            quit = 1;
        }
        
        
    }
           
}

void printDefinitionAllAlgorithm()
{
    char *algorithm_title;
	if(RMData.isValid == 1)
	{
		algorithm_title = "Rate Monothonic (RM)";
		definitionRM();
	}
	if(EDFData.isValid == 1)
	{
		algorithm_title = "Earliest Deadline First (EDF)";
		definitionEDF();
	}
	if(LLFData.isValid == 1)
	{
		algorithm_title = "Least Laxity First (LLF)";
		definitionLLF();
	}
}


void simulationStepByAlgorithm(RTSchedulerData *data, int step, char *algorithm, int cycles)
{

        fprintf(OUTPUT, "\\item %s\\\\", algorithm);
        fprintf(OUTPUT, "\\scalebox{0.7}{\\begin{tabular}{");
        for(int i = 0; i <= cycles + 1; i++)
        {
            fprintf(OUTPUT, "|c");
        }
        fprintf(OUTPUT, "|}\n \\multicolumn{1}{l}{} &" );
        for(int i = 1; i <= cycles ; i++)
        {
            fprintf(OUTPUT, "\\multicolumn{1}{l}{\\fontsize{2.5}{4}\\selectfont{%d}} %s", i , i < cycles? "&": "\\\\");
        }


        for(int i  = 0; i < data ->numProcess; i++)
        {
            fprintf(OUTPUT, "\\hline \n");
            fprintf(OUTPUT, "\\tiny Task %d &", i  +1);
            for(int j = 1; j < cycles + 1; j++)
            {   
                if(j <= step)
                {
                    if(data ->processList[j - 1] == i + 1)
                    {
                        if((j) % data ->deadline[i] == 0)
                        {
                            if(data ->deadProcess == (i  + 1) && j == data ->t_deadProcess + 1)
                            {
                                fprintf(OUTPUT, "\\multicolumn{1}{%sc!{\\color{red}\\vrule width 2pt}}{\\cellcolor{blue!25}}", j == 1 ? "|": "");
                                data ->isDone = 1;
                            }
                            else{

                                 fprintf(OUTPUT, "\\multicolumn{1}{%sc!{\\color{blue}\\vrule width 1pt}}{\\cellcolor{blue!25}}", j == 1? "|": "");
                            }
                           
                        }
                        else{
                            int k;
                            int tmp_j = j;
                            for(k =1; k <= data ->executionTime[i]; k++)
                            {   tmp_j += 1;
                                if(!(data ->processList[tmp_j - 1] == i + 1)) break;
                            }
                            j = tmp_j -1;

                            if((j) % data ->deadline[i] == 0)
                            {
                                if(data ->deadProcess == (i  + 1) && j == data ->t_deadProcess + 1)
                                {
                                    fprintf(OUTPUT, "\\multicolumn{%d}{c!{\\color{red}\\vrule width 2pt}}{\\cellcolor{blue!25}}", k);
                                    data ->isDone = 1;
                                }
                                else
                                {
                                    fprintf(OUTPUT, "\\multicolumn{%d}{c!{\\color{blue}\\vrule width 1pt}}{\\cellcolor{blue!25}}", k);
                                }
                                
                            }
                            else{

                                fprintf(OUTPUT, "\\multicolumn{%d}{c|}{\\cellcolor{blue!25}}", k);
                            }
                            
                        }
                    }
                    else
                    {
                        if(j% data ->deadline[i] == 0)
                        {  
                            if(data ->deadProcess == (i  + 1) && j == data ->t_deadProcess + 1)
                            {
                                fprintf(OUTPUT, "\\multicolumn{1}{%sc!{\\color{red}\\vrule width 2pt}}{}", j == 1 ? "|": "");
                                data ->isDone = 1;
                            }
                            else{

                                fprintf(OUTPUT, "\\multicolumn{1}{c!{\\color{blue}\\vrule width 1pt}}{}");
                            }
                            
                        }
                        else{

                            fprintf(OUTPUT, " ");
                        }
                    }
                }
                else{

                    if(j % data ->deadline[i] == 0)
                    {
                        fprintf(OUTPUT, "\\multicolumn{1}{c!{\\color{blue}\\vrule width 1pt}}{}");   
                    }
                    else{
                        fprintf(OUTPUT, " ");
                    }
                }
                if(!(j == cycles))
                {
                    fprintf(OUTPUT, "&");
                }
            }
            fprintf(OUTPUT, "\\\\\n");
            fprintf(OUTPUT, "\\hline \n");
        }
        
        fprintf(OUTPUT, "\\end{tabular}}\\newline");
}


void simulationAllAlgorithm(int cycles)
{
    printDefinitionAllAlgorithm();

    if(RMData.isValid){
        SchedulabilityTest(RMData.executionTime, RMData.deadline, RMData.numProcess);
    }
    else if(EDFData.isValid){
        SchedulabilityTest(EDFData.executionTime, EDFData.deadline, EDFData.numProcess);
    }
    else if(LLFData.isValid){
        SchedulabilityTest(LLFData.executionTime, LLFData.deadline, LLFData.numProcess);
    }

    for(int step = 1; step <=  cycles + 1; step ++)
    {
        fprintf(OUTPUT, "%s","\\begin{frame}[shrink=20]"); 
        fprintf(OUTPUT, "\\frametitle{%s - Time %d}", "Algorithm List", step, cycles);
        fprintf(OUTPUT, "\\begin{itemize}\n");

        if(RMData.isValid)
        {
            simulationStepByAlgorithm(&RMData, step, "Rate Monothonic (RM)", cycles);
        }

        if(EDFData.isValid)
        {
            simulationStepByAlgorithm(&EDFData, step, "Earliest Deadline First (EDF)", cycles);
        }
        
        if(LLFData.isValid)
        {
            simulationStepByAlgorithm(&LLFData, step, "Least Laxity First (LLF)", cycles);
        }


        fprintf(OUTPUT, "\\end{itemize}\n");
        fprintf(OUTPUT, "%s","\\end{frame}");

        if(LLFData.isDone == 1 && RMData.isDone == 1&& EDFData.isDone == 1)
            break;
    }
}







void createPresentation()
{
    InitPresentation();
    startPresentation();
}









