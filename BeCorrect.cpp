//***************************************************
//*                                                 *
//*                                                 *
//*   BeCorrect v 1.1.0. Last update: Nov 4, 2019   *
//*                                                 *
//*                                                 *
//***************************************************

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cctype>

using namespace std;

int usage();

int main(int argc, char* argv[])
{
    if (argc<4)
        return usage();
    
    FILE *rawcountfile, *adjcountfile, *bedgraphchrfile, *rawbedgraphfile, *adjbedgraphfile;
    
    char chrnames[10000][100], rawcountline[10000], adjcountline[10000], rawbedgraphline[10000], adjbedgraphline[10000], chr[2][10000], oldchr[10000], start[2][100], end[2][100], counts[2][100],adjbedgraphname[10000], bedgraphfield[1000], bedgraphchr[10000], oldbedgraphchr[10000], oldcountchr[10000], countchrname[1000], bedgraphchrname[1000];
    
    int intstart[2], intend[2], pos=0, on=0, skipcols=0, colsskipped=0, oldintstart=0, oldintend=0, rawbedgraphloc=0, bedgraphspan=0, bedgraphchron=0,rawbedgraphstart=0,rawbedgraphend=0, countchron=0,oldbedgraphloc=0;
    
    double rawbedgraphvalue=0.0, oldscalevalue=1.0, scalevalue=1.0, dblcounts[2];
    
    bool haschr=false;
    
    rawcountfile=fopen(argv[1],"r");
    adjcountfile=fopen(argv[2],"r");
    bedgraphchrfile=fopen(argv[3],"r");
    
    //Read the chr names
    pos=on=0;
    while(!feof(bedgraphchrfile))
    {
        fgets(bedgraphchrname,1000,bedgraphchrfile);
        on=0;
        while(!isspace(bedgraphchrname[on]) && bedgraphchrname[on]!=EOF)
        {
            on++;
        }
        bedgraphchrname[on]='\0';
        strcpy(chrnames[pos],bedgraphchrname);
        pos++;
    }
    
    //Go through each bedgraph file.
    for (int arg=4; arg<argc; arg++)
    {
        rawbedgraphfile=fopen(argv[arg],"r");
        //reset counts files to read through.
        rewind(rawcountfile);
        rewind(adjcountfile);
        
        //Read past first line of adj file
        fgets(adjcountline, 10000, adjcountfile);
    
        //Prep first line of data
        fgets(adjcountline,10000,adjcountfile);
        pos=on=0;
        while(adjcountline[pos]!='\t')
        {
            chr[1][on]=adjcountline[pos];
            on++;
            pos++;
        }
        pos++;
        chr[1][on]='\0';
        strcpy(oldcountchr,chr[1]);
        on=0;
        while(adjcountline[pos]!='\t')
        {
            start[1][on]=adjcountline[pos];
            on++;
            pos++;
        }
        pos++;
        start[1][on]='\0';
        intstart[1]=atoi(start[1]);
        on=0;
        while(adjcountline[pos]!='\t')
        {
            end[1][on]=adjcountline[pos];
            on++;
            pos++;
        }
            pos++;
        end[1][on]='\0';
        intend[1]=atoi(end[1]);
        on=0;
        colsskipped=0;
        while (colsskipped<skipcols)
        {
            if(adjcountline[pos]=='\t')
                colsskipped++;
            pos++;
        }
        while(adjcountline[pos]!='\t'  && adjcountline[pos]!='\n')
        {
            counts[1][on]=adjcountline[pos];
            on++;
            pos++;
        }
        pos++;
        counts[1][on]='\0';
        on=0;
        dblcounts[1]=atof(counts[1]);
        
        //Get name for out file from first line of raw file.
        fgets(rawcountline,10000,rawcountfile);
        pos=0;
        for (int fieldnum=0; fieldnum<arg-1;)
        {
            if(rawcountline[pos]=='\t')
            {
                fieldnum++;
            }
            pos++;
        }
        on=0;
        while(!isspace(rawcountline[pos]))
        {
            adjbedgraphname[on]=rawcountline[pos];
            on++;
            pos++;
        }
        adjbedgraphname[on]='\0';
        strcat(adjbedgraphname,"_adjusted.bg");
        fprintf(stdout, "Converting %s into %s\n", argv[arg],adjbedgraphname);
        adjbedgraphfile=fopen(adjbedgraphname,"w");
        
        fgets(rawcountline,10000,rawcountfile);
        pos=on=0;
        while(rawcountline[pos]!='\t')
        {
            chr[0][on]=rawcountline[pos];
            on++;
            pos++;
        }
        pos++;
        chr[0][on]='\0';
        on=0;
        while(rawcountline[pos]!='\t')
        {
            start[0][on]=rawcountline[pos];
            on++;
            pos++;
        }
        pos++;
        start[0][on]='\0';
        intstart[0]=atoi(start[0]);
        on=0;
        while(rawcountline[pos]!='\t')
        {
            end[0][on]=rawcountline[pos];
            on++;
            pos++;
        }
        pos++;
        end[0][on]='\0';
        intend[0]=atoi(end[0]);
        on=0;
        colsskipped=0;
        while (colsskipped<skipcols)
        {
            if(rawcountline[pos]=='\t')
                colsskipped++;
            pos++;
        }
        while(rawcountline[pos]!='\t' && rawcountline[pos]!= '\n')
        {
            counts[0][on]=rawcountline[pos];
            on++;
            pos++;
        }
        pos++;
        counts[0][on]='\0';
        dblcounts[0]=atof(counts[0]);
        on=0;
        while(strcmp(chr[0],chr[1])!=0 || intstart[0]!=intstart[1] || intend[0]!=intend[1])
        {
            fgets(rawcountline,10000,rawcountfile);
            pos=on=0;
            while(rawcountline[pos]!='\t')
            {
                chr[0][on]=rawcountline[pos];
                on++;
                pos++;
            }
            pos++;
            chr[0][on]='\0';
            on=0;
            while(rawcountline[pos]!='\t')
            {
                start[0][on]=rawcountline[pos];
                on++;
                pos++;
            }
            pos++;
            start[0][on]='\0';
            intstart[0]=atoi(start[0]);
            on=0;
            while(rawcountline[pos]!='\t')
            {
                end[0][on]=rawcountline[pos];
                on++;
                pos++;
            }
            pos++;
            end[0][on]='\0';
            intend[0]=atoi(end[0]);
            on=0;
            colsskipped=0;
            while (colsskipped<skipcols)
            {
                if(rawcountline[pos]=='\t')
                    colsskipped++;
                pos++;
            }
            while(rawcountline[pos]!='\t' && rawcountline[pos]!='\n')
            {
                counts[0][on]=rawcountline[pos];
                on++;
                pos++;
            }
            pos++;
            counts[0][on]='\0';
            dblcounts[0]=atof(counts[0]);
            on=0;
        }
        //Set 'old' values for default values .
        oldintend=oldintstart=0;
        strcpy(oldchr,chr[0]);
        
        scalevalue=1.0*(dblcounts[1]+1.0)/(dblcounts[0]+1.0);
        oldscalevalue=scalevalue;
        
        //Get the count chr on.
        countchron=0;
        while(strcmp(chr[1],chrnames[countchron])!=0)
        {
            countchron++;
        }
        //Have first matching values for raw counts and adj counts.  Ready to process bedgraph.
        
        //Read the bedgraph file.
        while(!feof(rawbedgraphfile))
        {
            fgets(rawbedgraphline,10000,rawbedgraphfile);
            //If we get a nondata line, we need to read past it.
            while(rawbedgraphline[0]!='c' || rawbedgraphline[1]!='h' || rawbedgraphline[2] != 'r')
            {
                fprintf(adjbedgraphfile,"%s",rawbedgraphline);
                fgets(rawbedgraphline,10000,rawbedgraphfile);
            }
            //get the chr
            pos=on=0;
            while(rawbedgraphline[pos]!='\t')
            {
                bedgraphchr[on]=rawbedgraphline[pos];
                pos++;
                on++;
            }
            bedgraphchr[on]='\0';
            on=0;
            pos++;
            bedgraphchron=0;
            while(strcmp(bedgraphchr,chrnames[bedgraphchron])!=0)
                bedgraphchron++;
            //Get the start position
            while(rawbedgraphline[pos]!='\t')
            {
                bedgraphfield[on]=rawbedgraphline[pos];
                on++;
                pos++;
            }
            bedgraphfield[on]='\0';
            on=0;
            pos++;
            rawbedgraphstart=atoi(bedgraphfield);
            //Get the end position
            while(rawbedgraphline[pos]!='\t')
            {
                bedgraphfield[on]=rawbedgraphline[pos];
                on++;
                pos++;
            }
            bedgraphfield[on]='\0';
            on=0;
            pos++;
            rawbedgraphend=atoi(bedgraphfield);
            //Get the center
            rawbedgraphloc=(rawbedgraphstart+rawbedgraphend)/2;
            //Get the value
            while(rawbedgraphline[pos]!='\n' && rawbedgraphline[pos]!=EOF)
            {
                bedgraphfield[on]=rawbedgraphline[pos];
                on++;
                pos++;
            }
            bedgraphfield[on]='\0';
            rawbedgraphvalue=atof(bedgraphfield);
            
            //Check position to see if we need a new count line.
            while(countchron<=bedgraphchron && rawbedgraphloc>intend[1]  && !feof(adjcountfile))
            {
                //move current values to old values.
                oldscalevalue=scalevalue;
                strcpy(oldchr,chr[0]);
                oldintend=intend[0];
                oldintstart=intstart[0];
                strcpy(oldcountchr,chr[1]);
                
                fgets(adjcountline,10000,adjcountfile);
                pos=on=0;
                while(adjcountline[pos]!='\t')
                {
                    chr[1][on]=adjcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                chr[1][on]='\0';
                countchron=0;
                while(strcmp(chr[1],chrnames[countchron])!=0)
                {
                    countchron++;
                }
                on=0;
                while(adjcountline[pos]!='\t')
                {
                    start[1][on]=adjcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                start[1][on]='\0';
                intstart[1]=atoi(start[1]);
                on=0;
                while(adjcountline[pos]!='\t')
                {
                    end[1][on]=adjcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                end[1][on]='\0';
                intend[1]=atoi(end[1]);
                on=0;
                colsskipped=0;
                while (colsskipped<skipcols)
                {
                    if(adjcountline[pos]=='\t')
                        colsskipped++;
                    pos++;
                }
                while(adjcountline[pos]!='\t' && adjcountline[pos]!='\n')
                {
                    counts[1][on]=adjcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                counts[1][on]='\0';
                on=0;
                dblcounts[1]=atof(counts[1]);
                
                fgets(rawcountline,10000,rawcountfile);
                pos=on=0;
                while(rawcountline[pos]!='\t')
                {
                    chr[0][on]=rawcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                chr[0][on]='\0';
                on=0;
                while(rawcountline[pos]!='\t')
                {
                    start[0][on]=rawcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                start[0][on]='\0';
                intstart[0]=atoi(start[0]);
                on=0;
                while(rawcountline[pos]!='\t')
                {
                    end[0][on]=rawcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                end[0][on]='\0';
                intend[0]=atoi(end[0]);
                on=0;
                colsskipped=0;
                while (colsskipped<skipcols)
                {
                    if(rawcountline[pos]=='\t')
                        colsskipped++;
                    pos++;
                }
                while(rawcountline[pos]!='\t' && rawcountline[pos]!='\n')
                {
                    counts[0][on]=rawcountline[pos];
                    on++;
                    pos++;
                }
                pos++;
                counts[0][on]='\0';
                dblcounts[0]=atof(counts[0]);
                on=0;
                while((strcmp(chr[0],chr[1])!=0 || intstart[0]!=intstart[1] || intend[0]!=intend[1])  && !feof(rawcountfile))
                {
                    fgets(rawcountline,10000,rawcountfile);
                    pos=on=0;
                    while(rawcountline[pos]!='\t')
                    {
                        chr[0][on]=rawcountline[pos];
                        on++;
                        pos++;
                    }
                    pos++;
                    chr[0][on]='\0';
                    on=0;
                    while(rawcountline[pos]!='\t')
                    {
                        start[0][on]=rawcountline[pos];
                        on++;
                        pos++;
                    }
                    pos++;
                    start[0][on]='\0';
                    intstart[0]=atoi(start[0]);
                    on=0;
                    while(rawcountline[pos]!='\t')
                    {
                        end[0][on]=rawcountline[pos];
                        on++;
                        pos++;
                    }
                    pos++;
                    end[0][on]='\0';
                    intend[0]=atoi(end[0]);
                    on=0;
                    colsskipped=0;
                    while (colsskipped<skipcols)
                    {
                        if(rawcountline[pos]=='\t')
                            colsskipped++;
                        pos++;
                    }
                    while(rawcountline[pos]!='\t' && rawcountline[pos]!='\n')
                    {
                        counts[0][on]=rawcountline[pos];
                        on++;
                        pos++;
                    }
                    pos++;
                    counts[0][on]='\0';
                    dblcounts[0]=atof(counts[0]);
                    on=0;
                }
                scalevalue=1.0*(dblcounts[1]+1.0)/(dblcounts[0]+1.0);
            }
            
            //With the right region, adjust bedgraph value!
            
            //Before the 'current' peak. Use scale value of weighted average between current and
            //old scale value dependent on how close to the nearest peaks we are.
            if(oldbedgraphloc!=rawbedgraphloc  && !feof(rawbedgraphfile))
            {
                if(rawbedgraphloc<intstart[0] && strcmp(chr[1],oldcountchr)==0 && strcmp(chr[1],bedgraphchr)==0)
                {
                    fprintf(adjbedgraphfile,"%s\t%d\t%d\t%f\n",bedgraphchr, rawbedgraphstart, rawbedgraphend, rawbedgraphvalue*((oldscalevalue*(rawbedgraphloc-oldintend)/(intstart[0]-oldintend))+(scalevalue*(intstart[0]-rawbedgraphloc)/(intstart[0]-oldintend))));
                }
                //In the peak. Scale by current scaling value.
                else if(rawbedgraphloc>=intstart[1] && rawbedgraphloc<=intend[1])
                {
                    fprintf(adjbedgraphfile,"%s\t%d\t%d\t%f\n", bedgraphchr, rawbedgraphstart, rawbedgraphend, rawbedgraphvalue*scalevalue);

                }
                //Before the first peak in chr. Scale by the first peak's factor.
                else if(rawbedgraphloc<intstart[1] && strcmp(chr[1],oldcountchr)!=0)
                {
                    fprintf(adjbedgraphfile,"%s\t%d\t%d\t%f\n",bedgraphchr, rawbedgraphstart, rawbedgraphend,rawbedgraphvalue*scalevalue);

                }
                //After the last peak in chr. Scale by the old peak's factor since it is the last
                //peak in the old chr.
                else if(rawbedgraphloc>intend[1])
                {
                    fprintf(adjbedgraphfile,"%s\t%d\t%d\t%f\n",bedgraphchr, rawbedgraphstart, rawbedgraphend, rawbedgraphvalue*oldscalevalue);
                }
                //This should be only tiggered if we hit a chr with no peaks?????? Don't scale
                else
                {
                    fprintf(adjbedgraphfile,"%s",rawbedgraphline);
                }
            }
        }
        fclose(adjbedgraphfile);
        fclose(rawbedgraphfile);
        skipcols++;
    }
    return 0;
}

int usage()
{
    fprintf(stdout,"\nBeCorrect (v1.0.0): A program to rescale bedgraph files to visually match batch correction using RUVSeq.\n");
    
    fprintf(stdout,"\n  Usage: BeCorrect <raw_counts> <ruv_adjusted_counts> <chr.names> 1.bg ... n.bg\n\n");
    
    fprintf(stdout,"Creates batch corrected bedgraph files based on batch correction performed by RUVseq on counts table for CHIPseq/ATACseq peaks.  For CHIP/ATAC, raw counts, adjusted counts, and bedgraph files should be sorted by chromosome then start location.\n\n");
    
    fprintf(stdout,"Sample counts file for CHIP/ATAC should be as follows:\nchr\tstart\tend\tname1\t...\tnameN\nName1\tstart1\tend1\tVal1.1\t...\tValN.1\nName2\tstart2\tend2\tVal1.2\t...\tValN.2\n\n");
    
    return 0;
}
