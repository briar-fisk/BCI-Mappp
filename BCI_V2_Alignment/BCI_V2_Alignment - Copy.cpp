// BCI_V2_Alignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#include <Windows.h>

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

    int Height = 5;
    int Width = 5;

    double Data[5] = { 1, 3, 1, 3, 1 };

public:

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
                std::cout << "[]";
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

    void output_Map()
    {
        std::cout << "\n Map_Vars:";
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

    c_Map Alignment[6];
    c_Map Replay[4];

    void output_Maps()
    {
        Alignment[0].output_Map();
        Alignment[1].output_Map();
        Alignment[2].output_Map();
        Alignment[3].output_Map();
        Alignment[4].output_Map();
        Alignment[5].output_Map();

        Replay[0].output_Map();
        Replay[1].output_Map();
        Replay[2].output_Map();
        Replay[3].output_Map();
    }
};

class c_Spike_Raster
{
public:

    int Depth;
    int Width;
    double* Raw;
    int** SR;
    int Range;
    int Range_Bool_Level;
    int* Out;
    double Render_Ratio;
    double* Out_Rendered;
    int* Out_Bool;

    c_Spike_Raster(int p_Range = 5, int p_Width = 5, int p_Depth = 100)
    {
        Raw = NULL;
        Out = NULL;
        Out_Rendered = NULL;
        Out_Bool = NULL;
        SR = NULL;
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

            Raw[cou_Width] = p_SR->Raw[cou_Width];
            Out[cou_Width] = p_SR->Out[cou_Width];
            Out_Rendered[cou_Width] = p_SR->Out_Rendered[cou_Width];
            Out_Bool[cou_Width] = p_SR->Out_Bool[cou_Width];
        }
    }

    void resize(int p_Width, int p_Depth)
    {
        if (Raw != NULL)
        {
            delete[] Raw;
            Raw = NULL;
        }

        if (Out != NULL)
        {
            delete[] Out;
            Out = NULL;
        }

        if (Out_Rendered != NULL)
        {
            delete[] Out_Rendered;
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

        for (int cou_Width = 0; cou_Width < Width; cou_Width++)
        {
            Raw[cou_Width] = rand() % Range;
            Out[cou_Width] = 0;
            Out_Rendered[cou_Width] = 0.0;
            Out_Bool[cou_Width] = 0;

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

        for (int cou_Depth = 0; cou_Depth < Depth; cou_Depth++)
        {
            if (SR[p_Index][cou_Depth])
            {
                ostr("A", 9);
            }
            if (!SR[p_Index][cou_Depth])
            {
                ostr("_", 12);
            }
        }
        std::cout << "| " << Out[p_Index];

        if (Out[p_Index] < 100)
        {
            std::cout << "  ";
        }



        /*
        if ((Out_Rendered[p_Index] - int(Out_Rendered[p_Index])) == 0)
        {
            std::cout << "  ";
        }*/
        
        std::cout << " " << Out_Bool[p_Index];

        std::cout << " ";

        for (int cou_Out = 0; cou_Out < Out[p_Index]; cou_Out++)
        {
            ostr("X", int(Out_Rendered[p_Index] + 7));
        }
        
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

class c_Agent
{
public:

    c_Map* Input;
    c_Map Output;

    double Nerve_Count;

    c_Spike_Raster Spike_Raster;
    
    //
    c_Spike_Raster * Spike_Raster_Snapshots;
    int Spike_Raster_Snapshots_Current;
    int Spike_Raster_Snapshots_Depth;
    
    c_Agent(int p_Nerve_Count = 5, int p_Snapshot_Count = 6)
    {
        Input = NULL;
        Spike_Raster_Snapshots = new c_Spike_Raster[p_Snapshot_Count];
        Spike_Raster_Snapshots_Current = 0;
        Spike_Raster_Snapshots_Depth = p_Snapshot_Count;
        for (int cou_Snp = 0; cou_Snp < p_Snapshot_Count; cou_Snp++)
        {
            Spike_Raster_Snapshots[cou_Snp].resize(5, 5);
        }
        Nerve_Count = p_Nerve_Count;
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

    void save_Snapshop()
    {
        if (Spike_Raster_Snapshots_Current < Spike_Raster_Snapshots_Depth)
        {
            c_Spike_Raster* tmp_Snp = &Spike_Raster_Snapshots[Spike_Raster_Snapshots_Current];
            tmp_Snp->copy_Raster(&Spike_Raster);

            Spike_Raster_Snapshots_Current++;
        }
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

    void output_Snapshots()
    {

        for (int cou_Snp = 0; cou_Snp < Spike_Raster_Snapshots_Current; cou_Snp++)
        {
            std::cout << "\nSnapshot: " << cou_Snp << " ";
            Spike_Raster_Snapshots[cou_Snp].output_Raster();
        }
    }
};

class c_Alignment_Module
{
public:


};

c_Mappings Mappings;
c_Spike_Raster Rasters;
c_Agent Agents[3];

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
    Mappings.Replay[0].set_Data(tmp_Data);

    tmp_Data[0] = 4; tmp_Data[1] = 3; tmp_Data[2] = 2; tmp_Data[3] = 1; tmp_Data[4] = 0;
    Mappings.Replay[1].set_Data(tmp_Data);

    tmp_Data[0] = 0; tmp_Data[1] = 2; tmp_Data[2] = 4; tmp_Data[3] = 2; tmp_Data[4] = 0;
    Mappings.Replay[2].set_Data(tmp_Data);

    tmp_Data[0] = 4; tmp_Data[1] = 1; tmp_Data[2] = 0; tmp_Data[3] = 1; tmp_Data[4] = 4;
    Mappings.Replay[2].set_Data(tmp_Data);


    tmp_Data[0] = 1; tmp_Data[1] = 1; tmp_Data[2] = 1; tmp_Data[3] = 1; tmp_Data[4] = 1;
    Mappings.Alignment[0].set_Data(tmp_Data);

    tmp_Data[0] = 3; tmp_Data[1] = 3; tmp_Data[2] = 3; tmp_Data[3] = 3; tmp_Data[4] = 3;
    Mappings.Alignment[1].set_Data(tmp_Data);

    tmp_Data[0] = 1; tmp_Data[1] = 3; tmp_Data[2] = 1; tmp_Data[3] = 3; tmp_Data[4] = 1;
    Mappings.Alignment[2].set_Data(tmp_Data);

    tmp_Data[0] = 1; tmp_Data[1] = 1; tmp_Data[2] = 1; tmp_Data[3] = 1; tmp_Data[4] = 3;
    Mappings.Alignment[3].set_Data(tmp_Data);

    tmp_Data[0] = 1; tmp_Data[1] = 1; tmp_Data[2] = 3; tmp_Data[3] = 3; tmp_Data[4] = 1;
    Mappings.Alignment[4].set_Data(tmp_Data);

    tmp_Data[0] = 1; tmp_Data[1] = 3; tmp_Data[2] = 1; tmp_Data[3] = 3; tmp_Data[4] = 1;
    Mappings.Alignment[5].set_Data(tmp_Data);

    Agents[0].Input = &(Mappings.Alignment[0]);

    return 1;
}

int Align()
{
    return 1;
}

int Playback()
{
    return 1;
}

int main()
{
    init();
    Output_Vars();
    Align();
    Playback();
    
    system("CLS");
    int tmp_Count = 0;
    int tmp_Map = 1;

    c_Map tmp_Input_Map;
    double tmp_Data[5];
    tmp_Data[0] = 1; tmp_Data[1] = 3; tmp_Data[2] = 1; tmp_Data[3] = 3; tmp_Data[4] = 1;
    tmp_Input_Map.set_Data(tmp_Data);

    while (1)
    {
        XY(0, 0);
        //std::cout << tmp_Count << " / " << 100;
        std::cout << " [";
        for (int cou_Index = 0; cou_Index < tmp_Count; cou_Index++)
        {
            std::cout << "X";
        }
        std::cout << ">";
        for (int cou_Index = tmp_Count; cou_Index < 100; cou_Index++)
        {
            std::cout << ".";
        }
        std::cout << "]\n\n";

        //Rasters.advance_Raster();
        //Rasters.output_Raster();
        Agents[0].update();
        Agents[0].output();

        //Sleep(10);

        tmp_Count++;


        if (tmp_Count == 100)
        {
            //tmp_Data[0] = (((rand() % int(2)) * 2) + 1); tmp_Data[1] = (((rand() % int(2)) * 2) + 1); tmp_Data[2] = (((rand() % int(2)) * 2) + 1); tmp_Data[3] = (((rand() % int(2)) * 2) + 1); tmp_Data[4] = (((rand() % int(2)) * 2) + 1);
            //tmp_Input_Map.set_Data(tmp_Data);

            Agents[0].Input = &(Mappings.Alignment[tmp_Map]);
            //Agents[0].Input = &tmp_Input_Map;
            tmp_Map++;

            if (tmp_Map >= 6) { tmp_Map = 0; }

            tmp_Count = 0;

            Agents[0].save_Snapshop();
            std::cout << "\n";
            Agents[0].output_Snapshots();
        }

        XY(0, 0);
    }
    
}
