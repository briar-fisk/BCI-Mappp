// BCI_V2_Alignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#include <bitset>
#include <Windows.h>
#include <time.h>

int Map_Count = 9;
int Map_Width = 5;
int Map_Depth = 5;
int Nerve_Count = 5;
int Chrono_Count = 3;
int Run_Count = 3;
int Spike_Raster_Depth = 10;
double Range = (Spike_Raster_Depth / 2) - 1;
double Range_Divide = Range / 2;

int Default_Color = 7;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void ostr(std::string p_Out, int p_Color)
{
    SetConsoleTextAttribute(hConsole, p_Color);
    std::cout << p_Out;
    SetConsoleTextAttribute(hConsole, Default_Color);
}

void oint(int p_Out, int p_Color)
{
    SetConsoleTextAttribute(hConsole, p_Color);
    std::cout << p_Out;
    SetConsoleTextAttribute(hConsole, Default_Color);

}

void XY(int p_X, int p_Y)
{
    COORD tmp_XY;
    tmp_XY.X = p_X;
    tmp_XY.Y = p_Y;
    SetConsoleCursorPosition(hConsole, tmp_XY);
}


class c_Map
{
private:


public:

    int Height = 5;
    int Width = 5;


    double Data[5] = { 1, 3, 1, 3, 1 };

    //Whether or not this map has been initialized.
    bool flg_Map_Set = false;

    c_Map()
    {
        Height = 5;
        Width = 5;
    }

    c_Map(int p_Height, int p_Width)
    {
        Height = p_Height;
        Width = p_Width;
    }

    c_Map(int p_Height, int p_Width, double p_Data[] )
    {
        Height = p_Height;
        Width = p_Width;

        for (int cou_Data = 0; cou_Data < Width; cou_Data++)
        {
            Data[cou_Data] = p_Data[cou_Data];
        }
    }

    void set_Data(double p_Data[])
    {
        for (int cou_Data = 0; cou_Data < Width; cou_Data++)
        {
            Data[cou_Data] = p_Data[cou_Data];
        }
        flg_Map_Set = true;
    }

    bool is_Set()
    {
        return flg_Map_Set;
    }

    double get_Data(int p_Index)
    {
        return Data[p_Index];
    }

    void output_Vars()
    {
        std::cout << "\n Map_Vars:";
        std::cout << "\n Height: " << Height;
        std::cout << "\n Width: " << Width;

        for (int cou_Data = 0; cou_Data < Width; cou_Data++)
        {
            std::cout << "\n Data[" << cou_Data << "]: " << Data[cou_Data];
        }
    }

    void output_Map_Row(int p_Row)
    {
        bool flg_Pre_Hit = true;
        std::cout << " " << Data[p_Row] << " | ";
        for (int cou_Out = 0; cou_Out < 5; cou_Out++)
        {
            if (cou_Out == Data[p_Row])
            {
                ostr("[]", (cou_Out + 9));
                //std::cout << "[]";
                flg_Pre_Hit = false;
            }
            if (cou_Out != Data[p_Row])
            {
                if (flg_Pre_Hit)
                {
                    std::cout << "--";
                }
                else
                {
                    std::cout << "  ";
                }
            }
            //std::cout << "|";
        }
        //std::cout << "|";
    }

    //186 ║
    //202 ╩
    /*
    void output_Map_Column(int p_Column)
    {
        bool flg_Pre_Hit = true;
        std::cout << " " << Data[p_Column] << " | ";
        std::string tmp_Hit = "" + char(202);
        //Generates the row and checks whether the current tile is under, at, or above the value of Data[].
        for (int cou_Row = 0; cou_Row < 5; cou_Row++)
        {
            if (p_Column == Data[cou_Row])
            {
                ostr(tmp_Hit, (cou_Out + 9));
                //std::cout << "[]";
                flg_Pre_Hit = false;
            }
            if (cou_Out != Data[p_Column])
            {
                if (flg_Pre_Hit)
                {
                    std::cout << "--";
                }
                else
                {
                    std::cout << "  ";
                }
            }
            //std::cout << "|";
        }
        //std::cout << "|";
    }*/

    void output_Map()
    {
        //std::cout << "\n Map_Vars:";
        std::cout << "\n Height: " << Height;
        std::cout << "\n Width: " << Width;

        for (int cou_Data = 0; cou_Data < Width; cou_Data++)
        {
            std::cout << "\n (" << cou_Data << ") ";
            output_Map_Row(cou_Data);
        }
    }
};


class c_Mappings
{
public:

    int Map_Count;
    c_Map * Maps;
    double * Input_Data;
    int Map_Width;
    int Map_Depth;

    c_Mappings(int p_Map_Count = 25, int p_Width = 5, int p_Depth = 5)
    {
        Map_Count = p_Map_Count;
        Maps = new c_Map[Map_Count];
        Input_Data = new double[p_Width];
        Map_Width = p_Width;
        Map_Depth = p_Depth;

        for (int cou_Index = 0; cou_Index < p_Width; cou_Index++)
        {
            Input_Data[cou_Index] = 0.0;
        }

        for (int cou_Index = 0; cou_Index < Map_Count; cou_Index++)
        {
            Maps[cou_Index].set_Data(Input_Data);
        }
    }

    void output_Maps()
    {
        /*
        Alignment[0].output_Map();
        Alignment[1].output_Map();
        Alignment[2].output_Map();
        Alignment[3].output_Map();
        Alignment[4].output_Map();
        //Alignment[5].output_Map();

        Replay[0].output_Map();
        Replay[1].output_Map();
        Replay[2].output_Map();
        Replay[3].output_Map();
        */
        for (int cou_Map = 0; cou_Map < Map_Count; cou_Map++)
        {
            if (Maps[cou_Map].is_Set())
            {
                std::cout << "\n Map[" << cou_Map << "]";
                Maps[cou_Map].output_Map();
            }
        }
    }
};

class c_Spike_Raster
{
public:

    int Depth;
    int Width;
    int Run_Depth;
    double* Raw;
    int** SR;
    int* SR_Sequence;
    int Range;
    int Range_Bool_Level;
    int* Out;
    double Render_Ratio;
    double* Out_Rendered;
    int* Out_Bool;

    c_Spike_Raster(int p_Range = 5, int p_Width = 5, int p_Depth = 100, int p_Run_Depth = 5)
    {
        Run_Depth = p_Run_Depth;
        Raw = NULL;
        Out = NULL;
        Out_Rendered = NULL;
        Out_Bool = NULL;
        SR = NULL;
        SR_Sequence = NULL;
        Depth = p_Depth;
        Width = p_Width;
        Range = p_Range;
        Range_Bool_Level = Range / 2;
        Render_Ratio = Depth / Range;
        if (Render_Ratio < 1) { Render_Ratio = 1; }
        resize(Width, Depth);
    }

    void copy_Raster(c_Spike_Raster* p_SR)
    {
        resize(p_SR->Width, p_SR->Depth);

        for (int cou_Width = 0; cou_Width < Width; cou_Width++)
        {
            for (int cou_Depth = 0; cou_Depth < Depth; cou_Depth++)
            {
                SR[cou_Width][cou_Depth] = p_SR->SR[cou_Width][cou_Depth];
            }

            Out[cou_Width] = p_SR->Out[cou_Width];
            Out_Rendered[cou_Width] = p_SR->Out_Rendered[cou_Width];
            Out_Bool[cou_Width] = p_SR->Out_Bool[cou_Width];

            Raw[cou_Width] = p_SR->Raw[cou_Width];

            SR_Sequence[cou_Width] = p_SR->SR_Sequence[cou_Width];
        }
    }

    void resize(int p_Width, int p_Depth)
    {
        if (Raw != NULL)
        {
            delete[] Raw;
            Raw = NULL;
        }

        if (SR_Sequence != NULL)
        {
            delete[] SR_Sequence;
            SR_Sequence = NULL;
        }

        if (Out != NULL)
        {
            delete [] Out;
            Out = NULL;
        }

        if (Out_Rendered != NULL)
        {
            delete [] Out_Rendered;
            Out_Rendered = NULL;
        }

        if (Out_Bool != NULL)
        {
            delete[] Out_Bool;
            Out_Bool = NULL;
        }

        if (SR != NULL)
        {
            for (int cou_Width = 0; cou_Width < Width; cou_Width++)
            {
                if (SR[cou_Width] != NULL)
                {
                    delete[] SR[cou_Width];
                    SR[cou_Width] = NULL;
                }
            }
            delete[] SR;
            SR = NULL;
        }

        Depth = p_Depth;
        Width = p_Width;

        Raw = new double[Width];
        Out = new int[Width];
        Out_Rendered = new double[Width];
        Out_Bool = new int[Width];
        SR = new int* [Width];
        SR_Sequence = new int [Width];

        for (int cou_Width = 0; cou_Width < Width; cou_Width++)
        {
            Raw[cou_Width] = rand() % Range;

            Out[cou_Width] = 0;
            Out_Rendered[cou_Width] = 0;
            Out_Bool[cou_Width] = 0;

            SR_Sequence[cou_Width] = 0;

            SR[cou_Width] = new int[Depth];
            for (int cou_Depth = 0; cou_Depth < Depth; cou_Depth++)
            {
                SR[cou_Width][cou_Depth] = ((rand() % (cou_Width + 1)) < cou_Width);
            }
        }
    }

    void generate_Spike(int p_Index)
    {
        SR[p_Index][Depth - 1] = ((rand() % Range) <= Raw[p_Index]);
    }

    void generate_Spikes()
    {
        for (int cou_Index = 0; cou_Index < Width; cou_Index++)
        {
            generate_Spike(cou_Index);
        }
    }

    void calculate_Output_Index(int p_Index)
    {
        Out_Rendered[p_Index] = ((double(Out[p_Index]) / Render_Ratio) - 1.0);

        if (Out_Rendered[p_Index] <= Range_Bool_Level)
        {
            Out_Bool[p_Index] = 0;
        }
        else
        {
            Out_Bool[p_Index] = 1;
        }
    }

    void calculate_Out_Index(int p_Index)
    {
        int tmp_Count = 0;

        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            if (SR[p_Index][cou_Index])
            {
                tmp_Count++;
            }
        }

        Out[p_Index] = tmp_Count;

        calculate_Output_Index(p_Index);
    }

    void calculate_Out()
    {
        for (int cou_Index = 0; cou_Index < Width; cou_Index++)
        {
            calculate_Out_Index(cou_Index);
        }
    }

    void shift_Spikes()
    {
        for (int cou_Index = 0; cou_Index < Width; cou_Index++)
        {
            for (int cou_D = 0; cou_D < (Depth - 1); cou_D++)
            {
                SR[cou_Index][cou_D] = SR[cou_Index][cou_D + 1];
            }
        }
    }

    void advance_Raster()
    {
        shift_Spikes();
        generate_Spikes();
        calculate_Out();
    }

    void output_Raster_Row(int p_Index)
    {
        std::cout << " " << Raw[p_Index] << " |";

        SR_Sequence[p_Index] = 0;
        for (int cou_Depth = 0; cou_Depth < Depth; cou_Depth++)
        {
            if (SR[p_Index][cou_Depth])
            {
                ostr("A", 10);
                //ostr("A", (SR_Sequence[p_Index] + 1));
                //SR_Sequence[p_Index]++;
            }
            if (!SR[p_Index][cou_Depth])
            {
                ostr("-", 12);
                SR_Sequence[p_Index] = 0;
            }
        }
        std::cout << "| " << Out[p_Index];

        if (Out[p_Index] < 100)
        {
            std::cout << " ";
        }



        /*
        if ((Out_Rendered[p_Index] - int(Out_Rendered[p_Index])) == 0)
        {
            std::cout << "  ";
        }*/
        
        std::cout << " " << Out_Bool[p_Index];

        std::cout << " ";

        std::string tmp_Output_X = "";
        for (int cou_Out = 0; cou_Out < (Out[p_Index] / 4); cou_Out++)
        {
            tmp_Output_X += "X";
        }

        ostr(tmp_Output_X, int(Out_Rendered[p_Index] + 7));

        std::cout << " " << Out_Rendered[p_Index];

        std::cout << "   ";
    }

    void output_Raster()
    {
        std::cout << " Spike Raster:";
        std::cout << " -Width:" << Width;
        std::cout << " -Depth:" << Depth;

        for (int cou_Width = 0; cou_Width < Width; cou_Width++)
        {
            std::cout << "\n(" << cou_Width << ")";
            output_Raster_Row(cou_Width);
        }
    }
};


//This handles the information needed for the alignment. 
//Raster information is gather
class c_Librarian_Module
{
public:

    double* Post_IO;

    //Sort by data, but store datas original index for the alignment layer.
    int* Sorted;
    std::string* Sorted_Strings;
    std::string* Alignment_Strings;

    double* Alignment_Layer;
    int BCI_Depth;

    c_Librarian_Module(int p_BCI_Depth = 5)
    {
        BCI_Depth = p_BCI_Depth;
        Post_IO = new double[BCI_Depth];
        Sorted = new int[BCI_Depth];
        Sorted_Strings = new std::string[BCI_Depth];
        Alignment_Strings = new std::string[BCI_Depth];
        Alignment_Layer = new double[BCI_Depth];
        wipe_Tables();
    }

    void wipe_Tables()
    {
        for (int cou_Index = 0; cou_Index < BCI_Depth; cou_Index++)
        {
            Post_IO[cou_Index] = 0.0;
            Sorted[cou_Index] = cou_Index;
            Sorted_Strings[cou_Index] = "";
            Alignment_Strings[cou_Index] = "10" + std::bitset<3>(cou_Index).to_string();
            Alignment_Layer[cou_Index] = cou_Index;
        }
    }

    //Bubble sort based on values stored in Sorted_Strings;
    void Sort()
    {
        bool flg_Not_Done = true;
        std::string tmp_Placeholder_Str = "";
        int tmp_Placeholder = 0;

        while (flg_Not_Done)
        {
            flg_Not_Done = false;

            for (int cou_Index = 0; cou_Index < (BCI_Depth - 1); cou_Index++)
            {
                if (Sorted_Strings[cou_Index] > Sorted_Strings[cou_Index + 1])
                {
                    flg_Not_Done = true;

                    tmp_Placeholder_Str = Sorted_Strings[cou_Index];
                    tmp_Placeholder = Sorted[cou_Index];

                    Sorted_Strings[cou_Index] = Sorted_Strings[cou_Index + 1];
                    Sorted[cou_Index] = Sorted[cou_Index + 1];
                    Sorted_Strings[cou_Index + 1] = tmp_Placeholder_Str;
                    Sorted[cou_Index + 1] = tmp_Placeholder;

                    tmp_Placeholder_Str = "";
                    tmp_Placeholder = 0;;
                }
            }
        }
    }

    void Align()
    {
        Sort();

        for (int cou_Index = 0; cou_Index < BCI_Depth; cou_Index++)
        {
            Alignment_Layer[cou_Index] = Sorted[cou_Index];
        }
    }

    void output_Table_Row(int p_Index)
    {
        //ostr("|", 9); std::cout << "Post_IO: " << Post_IO[p_Index]; ostr("|", 9);
        ostr("|", 9); std::cout << "Sort_In_BCI: " << Sorted[p_Index]; ostr("|", 9);
        ostr("|", 9); std::cout << "Sort_In_Str: " << Sorted_Strings[p_Index]; ostr("|", 9);
        ostr("|", 9); std::cout << "Align_Str: " << Alignment_Strings[p_Index]; ostr("|", 9);
        ostr("|", 9); std::cout << "Align_Layer: " << Alignment_Layer[p_Index]; ostr("|", 9);
    }

    void output_Tables()
    {
        for (int cou_Index = 0; cou_Index < BCI_Depth; cou_Index++)
        {
            std::cout << "\n (" << cou_Index << ") ";
            output_Table_Row(cou_Index);
        }
    }

};

class c_Raster_Database
{
public:

    c_Spike_Raster* Rasters;
    int Raster_Depth_Max;
    int Raster_Depth_Current;

    c_Map* Raster_Maps;

    c_Raster_Database(int p_Raster_Depth_Max = 10)
    {
        Raster_Depth_Current = 0;
        Raster_Depth_Max = p_Raster_Depth_Max;

        Rasters = new c_Spike_Raster[Raster_Depth_Max];
        Raster_Maps = new c_Map[Raster_Depth_Max];
    }

    void submit_Raster(c_Spike_Raster* p_SR, int p_Raster_ID = -1)
    {
        if (p_Raster_ID != -1)
        {
            Rasters[p_Raster_ID].copy_Raster(p_SR);
            Raster_Maps[p_Raster_ID].set_Data(p_SR->Raw);
        }
        else
        {
            Rasters[Raster_Depth_Current].copy_Raster(p_SR);
            Raster_Maps[Raster_Depth_Current].set_Data(p_SR->Raw);
            Raster_Depth_Current++;
        }
    }

    void output_Rasters()
    {
        for (int cou_Index = 0; cou_Index < Raster_Depth_Current; cou_Index++)
        {
            std::cout << "\nDB_Raster[" << cou_Index << "]:";
            //Rasters[cou_Index].output_Raster();


            for (int cou_Row = 0; cou_Row < Rasters[cou_Index].Width; cou_Row++)
            {
                std::cout << "\n(" << cou_Row << ") ";

                Raster_Maps[cou_Index].output_Map_Row(cou_Row);

                Rasters[cou_Index].output_Raster_Row(cou_Row);
            }
        }
    }
         
};

class c_Agent
{
public:

    c_Librarian_Module Librarian_Module;

    c_Map* Input;
    c_Map Output;

    int Nerve_Count;

    c_Spike_Raster Spike_Raster;
    
    c_Spike_Raster * Spike_Raster_Snapshots;
    int Spike_Raster_Snapshots_Current;
    int Spike_Raster_Snapshots_Depth;
    c_Map* Spike_Raster_Snapshots_Maps;

    int* BCI;

    c_Agent(int p_Nerve_Count = 5, int p_Snapshot_Count = 10)
    {
        Input = NULL;
        Spike_Raster_Snapshots = new c_Spike_Raster[p_Snapshot_Count];
        Spike_Raster_Snapshots_Maps = new c_Map[p_Snapshot_Count];
        Spike_Raster_Snapshots_Current = 0;
        Spike_Raster_Snapshots_Depth = p_Snapshot_Count;
        for (int cou_Snp = 0; cou_Snp < p_Snapshot_Count; cou_Snp++)
        {
            Spike_Raster_Snapshots[cou_Snp].resize(5, 100);
        }
        Nerve_Count = p_Nerve_Count;
        BCI = new int[Nerve_Count];
        for (int cou_N = 0; cou_N < Nerve_Count; cou_N++)
        {
            BCI[cou_N] = cou_N;
        }
        srand((unsigned int)(time(NULL)));
        scramble_BCI();
    }

    void scramble_BCI(int p_Passes = 100)
    {
        //Temp variable for BCI Scrambler
        int tmp_BCI = 0;
        int tmp_BCI_Swap_ID = 0;

        for (int cou_Count = 0; cou_Count < p_Passes; cou_Count++)
        {
            for (int cou_N = 0; cou_N < Nerve_Count; cou_N++)
            {
                std::cout << "\n ___" << cou_N;
                tmp_BCI = BCI[cou_N];

                tmp_BCI_Swap_ID = (rand() % int(Nerve_Count));

                BCI[cou_N] = BCI[tmp_BCI_Swap_ID];

                BCI[tmp_BCI_Swap_ID] = tmp_BCI;

                tmp_BCI = 0;
            }
        }
        
    }

    void output_BCI_Index(int p_Index)
    {
        std::cout << "|BCI: " << BCI[p_Index] << "|";
    }

    void output_BCI()
    {
        for (int cou_Index = 0; cou_Index < Nerve_Count; cou_Index++)
        {
            std::cout << "\n (" << cou_Index << ") ";
            output_BCI_Index(cou_Index);
        }
    }

    void get_Input()
    {
        if (Input == NULL) { return; }
        for (int cou_Index = 0; cou_Index < Nerve_Count; cou_Index++)
        {
            Spike_Raster.Raw[cou_Index] = Input->get_Data(cou_Index);
        }
    }

    void read_Output_From_Raster()
    {
        double * tmp_Out;
        tmp_Out = new double[int(Nerve_Count)];
        for (int cou_Index = 0; cou_Index < Nerve_Count; cou_Index++)
        {
            tmp_Out[cou_Index] = Spike_Raster.Out_Rendered[cou_Index];
        }
        Output.set_Data(tmp_Out);
    }

    void update()
    {
        get_Input();
        Spike_Raster.advance_Raster();
        read_Output_From_Raster();
    }

    //Updates a raster after the BCI spikes are added. Doesn't advance, only adds and recalculates.
    void update_Add_Spikes()
    {
        Spike_Raster.calculate_Out();
        read_Output_From_Raster();
    }

    void save_Snapshop(int p_Snapshot_ID = -1)
    {
        if ((Spike_Raster_Snapshots_Current < Spike_Raster_Snapshots_Depth) && (p_Snapshot_ID == -1))
        {
            c_Spike_Raster* tmp_Snp = &Spike_Raster_Snapshots[Spike_Raster_Snapshots_Current];
            tmp_Snp->copy_Raster(&Spike_Raster);
            Spike_Raster_Snapshots_Maps[Spike_Raster_Snapshots_Current].set_Data(tmp_Snp->Raw);

            Spike_Raster_Snapshots_Current++;
        }
        if ((p_Snapshot_ID != -1) && (p_Snapshot_ID < Spike_Raster_Snapshots_Depth))
        {
            if (Spike_Raster_Snapshots_Current <= p_Snapshot_ID) { Spike_Raster_Snapshots_Current = (p_Snapshot_ID + 1); }

            c_Spike_Raster* tmp_Snp = &Spike_Raster_Snapshots[p_Snapshot_ID];
            tmp_Snp->copy_Raster(&Spike_Raster);
            Spike_Raster_Snapshots_Maps[p_Snapshot_ID].set_Data(tmp_Snp->Raw);
        }
    }

    //Get the results from the raster and feed them into the librarian implant.
    void gather_Alignment(int p_Run, int p_Snapshot)
    {
        //The results are passed through the BCI to scramble them prior to submission to the Librarian_Implant.
        for (int cou_Index = 0; cou_Index < Nerve_Count; cou_Index++)
        {
            if (Spike_Raster_Snapshots[p_Snapshot].Out_Bool[cou_Index])
            {
                Librarian_Module.Sorted_Strings[BCI[cou_Index]] += "1";
            }
            else
            {
                Librarian_Module.Sorted_Strings[BCI[cou_Index]] += "0";
            }
        }
    }

    void reset_Snapshots()
    {

        for (int cou_Map = 0; cou_Map < Spike_Raster_Snapshots_Current; cou_Map++)
        {
            Spike_Raster_Snapshots[cou_Map].resize(Nerve_Count, Spike_Raster_Depth);
        }
        Spike_Raster_Snapshots_Current = 0;
    }

    void output()
    {

        for (int cou_Row = 0; cou_Row < Nerve_Count; cou_Row++)
        {
            std::cout << "\n(" << cou_Row << ") ";
            if (Input != NULL)
            {
                Input->output_Map_Row(cou_Row);
            }
            Spike_Raster.output_Raster_Row(cou_Row);

            //Output.output_Map_Row(cou_Row);
        }
    }

    void output_With_BCI()
    {

        for (int cou_Row = 0; cou_Row < Nerve_Count; cou_Row++)
        {
            std::cout << "\n(" << BCI[cou_Row] << ") ";
            if (Input != NULL)
            {
                Input->output_Map_Row(BCI[cou_Row]);
            }
            Spike_Raster.output_Raster_Row(BCI[cou_Row]);

            //Output.output_Map_Row(cou_Row);
        }
    }

    void output_Snapshots_With_BCI()
    {
        for (int cou_Snp = 0; cou_Snp < Spike_Raster_Snapshots_Current; cou_Snp++)
        {
            std::cout << "\nSnapshot: " << cou_Snp << " ";

            for (int cou_Row = 0; cou_Row < Nerve_Count; cou_Row++)
            {
                std::cout << "\n(" << BCI[cou_Row] << ") ";

                Spike_Raster_Snapshots_Maps[cou_Snp].output_Map_Row(BCI[cou_Row]);

                Spike_Raster_Snapshots[cou_Snp].output_Raster_Row(BCI[cou_Row]);
            }
        }
    }

    void output_Snapshots()
    {
        for (int cou_Snp = 0; cou_Snp < Spike_Raster_Snapshots_Current; cou_Snp++)
        {
            std::cout << "\nSnapshot: " << cou_Snp << " ";

            for (int cou_Row = 0; cou_Row < Nerve_Count; cou_Row++)
            {
                std::cout << "\n(" << cou_Row << ") ";

                Spike_Raster_Snapshots_Maps[cou_Snp].output_Map_Row(cou_Row);

                Spike_Raster_Snapshots[cou_Snp].output_Raster_Row(cou_Row);
            }
        }
    }

    void output_Alignment_Sequence()
    {
        for (int cou_Row = 0; cou_Row < Nerve_Count; cou_Row++)
        {
            std::cout << "\n(" << cou_Row << ") ";

            output_BCI_Index(cou_Row);
            Librarian_Module.output_Table_Row(cou_Row);
        }
    }
};


c_Mappings Mappings;
c_Spike_Raster Rasters;
c_Agent Agents[3];
c_Raster_Database Raster_DB;

int Output_Vars()
{
    std::cout << "\n Vars:";
    std::cout << "\n -Chrono_Count: " << Chrono_Count;
    std::cout << "\n -Run_Count: " << Run_Count;
    std::cout << "\n -Spike_Raster_Depth: " << Spike_Raster_Depth;
    std::cout << "\n -Range: " << Range;
    std::cout << "\n -Range_Divide: " << Range_Divide;

    Mappings.output_Maps();
    Rasters.output_Raster();

    return 1;
}

int init()
{
    double tmp_Data[5] = { 0, 1, 2, 3, 4 };
    Mappings.Maps[0].set_Data(tmp_Data);

    tmp_Data[0] = 4; tmp_Data[1] = 3; tmp_Data[2] = 2; tmp_Data[3] = 1; tmp_Data[4] = 0;
    Mappings.Maps[1].set_Data(tmp_Data);

    tmp_Data[0] = 0; tmp_Data[1] = 2; tmp_Data[2] = 4; tmp_Data[3] = 2; tmp_Data[4] = 0;
    Mappings.Maps[2].set_Data(tmp_Data);

    tmp_Data[0] = 4; tmp_Data[1] = 1; tmp_Data[2] = 0; tmp_Data[3] = 1; tmp_Data[4] = 4;
    Mappings.Maps[3].set_Data(tmp_Data);

    
    //High and Low baselines.
    tmp_Data[0] = 3; tmp_Data[1] = 3; tmp_Data[2] = 3; tmp_Data[3] = 3; tmp_Data[4] = 3;
    Mappings.Maps[4].set_Data(tmp_Data);

    tmp_Data[0] = 1; tmp_Data[1] = 1; tmp_Data[2] = 1; tmp_Data[3] = 1; tmp_Data[4] = 1;
    Mappings.Maps[5].set_Data(tmp_Data);

    //Binary alignment sequences
    tmp_Data[0] = 1; tmp_Data[1] = 1; tmp_Data[2] = 1; tmp_Data[3] = 1; tmp_Data[4] = 3;
    Mappings.Maps[6].set_Data(tmp_Data);

    tmp_Data[0] = 1; tmp_Data[1] = 1; tmp_Data[2] = 3; tmp_Data[3] = 3; tmp_Data[4] = 1;
    Mappings.Maps[7].set_Data(tmp_Data);

    tmp_Data[0] = 1; tmp_Data[1] = 3; tmp_Data[2] = 1; tmp_Data[3] = 3; tmp_Data[4] = 1;
    Mappings.Maps[8].set_Data(tmp_Data);

    for (int cou_Agent = 0; cou_Agent < 3; cou_Agent++)
    {
        Agents[cou_Agent].Input = &(Mappings.Maps[10]);
    }

    return 1;
}

void output_Loading_Bar(int p_Count, int flg_Position = 1)
{

    if (flg_Position) { XY(0, 0); }
    //std::cout << tmp_Count << " / " << 100;
    std::cout << " [";
    for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
    {
        std::cout << "X";
    }
    std::cout << ">";
    for (int cou_Index = p_Count; cou_Index < 100; cou_Index++)
    {
        std::cout << ".";
    }
    std::cout << "]";

}

//Maps [4 - 8]
//This runs through the alignment sequence.
//Starting with looping through each map and generating the rasters.
//After each map it gathers the boolean output to compare to the alignment strings.
//The data is then sorted and an alignment layer is generated.
int Align(int p_Agent)
{
    //for (int cou_Run=0;cou_Run<Run_Count;cou_Run++)
    //{
        for (int cou_Map = 4; cou_Map < 9; cou_Map++)
        //for (int cou_Map = 6; cou_Map < 9; cou_Map++)
        {
            Agents[p_Agent].Input = &(Mappings.Maps[cou_Map]);

            for (int cou_Count = 0; cou_Count < 100; cou_Count++)
            {
                XY(0, 0);
                std::cout << " Aligning: ";
                output_Loading_Bar(cou_Count, 0);
                std::cout << " Agent: " << (p_Agent + 1) << " / 3";
                std::cout << " Map: " << cou_Map - 3 << " / 5";
                //std::cout << " Run: " << cou_Run + 1 << " / " << Run_Count << " Map: " << cou_Map - 3 << " / 5";
                Agents[p_Agent].update();
                Agents[p_Agent].output();
                std::cout << "\n With BCI Scrambling:";
                Agents[p_Agent].output_With_BCI();
            }

            // -4 to account for the starting offset in the map array.
            Agents[p_Agent].save_Snapshop(cou_Map - 4);
            //Agents[p_Agent].gather_Alignment(cou_Run, (cou_Map - 4));
            Agents[p_Agent].gather_Alignment(0, (cou_Map - 4));
            Agents[p_Agent].Librarian_Module.output_Tables();
            Agents[p_Agent].output_Snapshots_With_BCI();

        }

        Agents[p_Agent].Librarian_Module.Align();
        //Agents[p_Agent].Librarian_Module.output_Tables();
        ostr("\n\n Alignment Sequence results:\n", 12);
        Agents[p_Agent].output_Alignment_Sequence();
        std::cout << "\n\n";
        //system("PAUSE");

        Sleep(250);
        //system("CLS");
        Agents[p_Agent].reset_Snapshots();
    //}
    return 1;
}

//Maps [0 - 3]
int record_Playback_Rasters(int p_Agent)
{

    //Record the rasters from the first agent.
    for (int cou_Map = 0; cou_Map < 3; cou_Map++)
    {
        Agents[p_Agent].Input = &(Mappings.Maps[cou_Map]);

        for (int cou_Count = 0; cou_Count < 100; cou_Count++)
        {
            XY(0, 0);
            std::cout << "Recording Playback Rasters: ";
            output_Loading_Bar(cou_Count, 0);
            std::cout << " Agent[" << p_Agent << "]";
            std::cout << " Map: " << (cou_Map + 1) << " / 3";
            Agents[p_Agent].update();
            Agents[p_Agent].output();
            std::cout << "\n With BCI Scrambling:";
            Agents[p_Agent].output_With_BCI();
        }

        // -4 to account for the starting offset in the map array.
        Agents[p_Agent].save_Snapshop(cou_Map);
        Raster_DB.submit_Raster(&(Agents[p_Agent].Spike_Raster_Snapshots[Agents[p_Agent].Spike_Raster_Snapshots_Current - 1]));
        Agents[p_Agent].output_Snapshots();
    }

    Sleep(250);
    //system("CLS");
    //Agents[p_Agent].reset_Snapshots();

    return 1;
}


int playback_Raster(int p_Agent, int p_Raster)
{
    int tmp_Depth = Raster_DB.Rasters[p_Raster].Depth;

    //For raster depth step through adding the current column to the agents BCI output.
    for (int cou_Col = 0; cou_Col < tmp_Depth; cou_Col++)
    {
        //std::cout << "\nY(" << cou_Col << ") ";
        Agents[p_Agent].update();

        for (int cou_Row = 0; cou_Row < Raster_DB.Rasters[p_Raster].Width; cou_Row++)
        {
            //std::cout << " " << Raster_DB.Rasters[p_Raster].SR[cou_Row][cou_Col];
            if (Agents[p_Agent].Spike_Raster.SR[int(Agents[p_Agent].Librarian_Module.Alignment_Layer[cou_Row])][tmp_Depth - 1]) { continue; }
            //The unscrabled source is correct. The alignment layer places the data where the BCI will then unscrable it.
            Agents[p_Agent].Spike_Raster.SR[int(Agents[p_Agent].Librarian_Module.Alignment_Layer[cou_Row])][tmp_Depth - 1] = Raster_DB.Rasters[p_Raster].SR[cou_Row][cou_Col];
        }
        Agents[p_Agent].update_Add_Spikes();
        XY(0, 0);
        Agents[p_Agent].output();
        std::cout << "\n With BCI Scrambling:";
        Agents[p_Agent].output_With_BCI();
        
    }
    return 1;
}

int playback_Rasters(int p_Agent)
{
    system("CLS");
    for (int cou_Raster = 0; cou_Raster < Raster_DB.Raster_Depth_Current; cou_Raster++)
    {
        system("CLS");
        std::cout << " Playing back Raster[" << cou_Raster << "] for Agent[" << p_Agent << "]";
        playback_Raster(p_Agent, cou_Raster);
    }
    return 1;
}

int Playback()
{
    //Records the rasters from the first agent using the playback mappings.
    record_Playback_Rasters(0);

    //After the rasters are recorded in the database they are then put through the Librarian_Module in agents 1 & 2.
    //The output is then available to see if it lines up.
    playback_Rasters(1);
    playback_Rasters(2);

    return 1;
}

void final_Presentation_Alignment(int p_Agent)
{
    //Map
    for (int cou_Height = 0; cou_Height < Agents[p_Agent].Input->Height; cou_Height++)
    {
        //Agents[p_Agent].Input->output_Map_Column(cou_Height);
    }

    //Raw
    

    //SR
    //BCI
    //Post
    //Align
}

void final_Presentation_Playback(int p_Agent)
{
    //Show the database raster
    //Alignment Layer
    //Post
    //BCI
    //Raster
    //Raw
}

//186 ║
//202 ╩
//179 │
//215 ╫
//202 

void final_Presentation()
{
    system("CLS");

    std::cout << "Results:";

    //Start with alignment. Show the alignment tables.
    for (int cou_Agent = 0; cou_Agent < 3; cou_Agent++)
    {
        std::cout << "\n\n Agent[" << cou_Agent << "] Alignment Tables:";
        final_Presentation_Alignment(cou_Agent);
    }
    
    //Then the playback
    for (int cou_Agent = 0; cou_Agent < 3; cou_Agent++)
    {
        std::cout << "\n\n Agent[" << cou_Agent << "] Playback Tables:";
        final_Presentation_Playback(cou_Agent);
    }
}

int main()
{
    /*
    for (int cou_Index = 0; cou_Index < 255; cou_Index++)
    {
        std::cout << "\n" << cou_Index << " --- " << char(cou_Index);
    }
    system("pause");
    */
    srand((unsigned int)(time(NULL)));
    init();
    Output_Vars();

    for (int cou_Agent = 0; cou_Agent < 3; cou_Agent++)
    {
        Agents[cou_Agent].output_BCI();
        Agents[cou_Agent].Librarian_Module.output_Tables();
    }

    system("CLS");


    int tmp_Count = 0;
    int tmp_Map = 0;

    c_Map tmp_Input_Map;
    double tmp_Data[5];
    tmp_Data[0] = 1; tmp_Data[1] = 1; tmp_Data[2] = 1; tmp_Data[3] = 1; tmp_Data[4] = 1;
    tmp_Input_Map.set_Data(tmp_Data);

    for (int cou_Agent = 0; cou_Agent < 1; cou_Agent++)
    {
        Agents[cou_Agent].Input = &(Mappings.Maps[tmp_Map]);

        Align(cou_Agent);

        for (int cou_Count = 0; cou_Count < 100; cou_Count++)
        {
            XY(0, 0);
            std::cout << "Playback Starting: ";
            output_Loading_Bar(cou_Count, 0);
            std::cout << " Agent: " << (cou_Agent + 1) << " / 3";
            Sleep(10);
        }

        system("CLS");
    }

    //Record the raster from agent 0.
    Playback();
    std::cout << "\n\n\nDatabase:\n\n";
    Raster_DB.output_Rasters();

    //if (cou_Agent < 2) { system("CLS"); }

    //final_Presentation();

    std::cout << "\n\n\n";
    system("pause");
    system("exit");
}
