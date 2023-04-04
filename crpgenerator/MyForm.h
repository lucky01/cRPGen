#pragma once

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <tchar.h>
#include <conio.h>
#define ZLIB_WINAPI
#include "zlib.h"
#include "cRom.h"

HWND hWnd;
HDC hdc;
COLORREF *arr; 

gzFile gzinfile, gzoutfile;
FILE *infile;
char* srcfileName;
int lasttick, tick;
int duration, nSrcFrames = 0;
long fileSize, framesize, position;
int ticksize = 0x0c;
bool isOpen;
bool render16 = false;
int srcWidth = 0;
int srcHeight = 0;
int serumWidth = 0;
int serumHeight = 0;
int nFrames = 0;
int detectedFrames = 0;
int skippedFrames = 0;
cRP_struct MycRP = { "",{FALSE},{0},0,0,{0},FALSE,0,FALSE };

static unsigned char buffer[256 * 64];

HMODULE hModule;

typedef bool(*Serum_Load_t)(const char* const altcolorpath, const char* const romname, int* pwidth, int* pheight, unsigned int* pnocolors, unsigned int* pntriggers);
Serum_Load_t Serum_Load;

typedef void(*Serum_Dispose_t)(void);
Serum_Dispose_t Serum_Dispose;

typedef UINT32(*Serum_GetNFrames_t)();
Serum_GetNFrames_t Serum_GetNFrames;

typedef int(*Serum_IdentifyFrame_t)(UINT8* frame);
Serum_IdentifyFrame_t Serum_IdentifyFrame;


namespace crpgenerator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ colorized;
	private: System::Windows::Forms::Label^ detected;
	private: System::Windows::Forms::Label^ skipped;
	private: System::Windows::Forms::Button^ load_dump;

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::Button^ load_crz;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ status;


	private: System::ComponentModel::IContainer^  components;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->load_dump = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->load_crz = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->colorized = (gcnew System::Windows::Forms::Label());
			this->detected = (gcnew System::Windows::Forms::Label());
			this->skipped = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->status = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// load_dump
			// 
			this->load_dump->Enabled = false;
			this->load_dump->Location = System::Drawing::Point(130, 12);
			this->load_dump->Name = L"load_dump";
			this->load_dump->Size = System::Drawing::Size(95, 23);
			this->load_dump->TabIndex = 6;
			this->load_dump->Text = L"LOAD DUMP";
			this->load_dump->UseVisualStyleBackColor = true;
			this->load_dump->Click += gcnew System::EventHandler(this, &MyForm::load_dump_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// load_crz
			// 
			this->load_crz->Location = System::Drawing::Point(12, 12);
			this->load_crz->Name = L"load_crz";
			this->load_crz->Size = System::Drawing::Size(95, 23);
			this->load_crz->TabIndex = 9;
			this->load_crz->Text = L"LOAD CRZ";
			this->load_crz->UseVisualStyleBackColor = true;
			this->load_crz->Click += gcnew System::EventHandler(this, &MyForm::load_crz_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(24, 50);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(83, 13);
			this->label1->TabIndex = 8;
			this->label1->Text = L"colorized frames";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(24, 66);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(83, 13);
			this->label2->TabIndex = 10;
			this->label2->Text = L"detected frames";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(29, 82);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(78, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"skipped frames";
			// 
			// colorized
			// 
			this->colorized->AutoSize = true;
			this->colorized->Location = System::Drawing::Point(175, 50);
			this->colorized->Name = L"colorized";
			this->colorized->Size = System::Drawing::Size(13, 13);
			this->colorized->TabIndex = 12;
			this->colorized->Text = L"0";
			// 
			// detected
			// 
			this->detected->AutoSize = true;
			this->detected->Location = System::Drawing::Point(175, 66);
			this->detected->Name = L"detected";
			this->detected->Size = System::Drawing::Size(13, 13);
			this->detected->TabIndex = 13;
			this->detected->Text = L"0";
			// 
			// skipped
			// 
			this->skipped->AutoSize = true;
			this->skipped->Location = System::Drawing::Point(175, 82);
			this->skipped->Name = L"skipped";
			this->skipped->Size = System::Drawing::Size(13, 13);
			this->skipped->TabIndex = 14;
			this->skipped->Text = L"0";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(247, 17);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(37, 13);
			this->label4->TabIndex = 15;
			this->label4->Text = L"Status";
			// 
			// status
			// 
			this->status->AutoSize = true;
			this->status->Location = System::Drawing::Point(290, 17);
			this->status->Name = L"status";
			this->status->Size = System::Drawing::Size(19, 13);
			this->status->TabIndex = 16;
			this->status->Text = L"ok";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(375, 112);
			this->Controls->Add(this->status);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->skipped);
			this->Controls->Add(this->detected);
			this->Controls->Add(this->colorized);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->load_crz);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->load_dump);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"MyForm";
			this->Text = L"cRPGenerator";
			this->Closed += gcnew System::EventHandler(this, &MyForm::MyForm_Closed);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	void DoEvents()
	{
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				break;
		}
	}

	void Free_cRP(void)
	{
		// Free buffers for MycRP
		if (MycRP.name[0] != 0)
		{
			MycRP.name[0] = 0;
			// Nothing to free for now
		}
		if (MycRP.oFrames)
		{
			free(MycRP.oFrames);
			MycRP.oFrames = NULL;
		}
		if (MycRP.FrameDuration)
		{
			free(MycRP.FrameDuration);
			MycRP.FrameDuration = NULL;
		}
	}

	bool Save_cRP(bool autosave)
	{
		if (MycRP.name[0] == 0) return true;
		char tbuf[MAX_PATH];
		if (!autosave) sprintf_s(tbuf, MAX_PATH, "%s%s.cRP", MycRP.SaveDir, MycRP.name); else sprintf_s(tbuf, MAX_PATH, "%s%s(auto).cRP", MycRP.SaveDir, MycRP.name);
		FILE* pfile;
		if (fopen_s(&pfile, tbuf, "wb") != 0)
		{
			return false;
		}
		fwrite(MycRP.name, 1, 64, pfile);
		fwrite(MycRP.oFrames, 1, nFrames * serumWidth * serumHeight, pfile);
		fwrite(MycRP.activeColSet, sizeof(BOOL), MAX_COL_SETS, pfile);
		fwrite(MycRP.ColSets, sizeof(UINT8), MAX_COL_SETS * 16, pfile);
		fwrite(&MycRP.acColSet, sizeof(UINT8), 1, pfile);
		fwrite(&MycRP.preColSet, sizeof(UINT8), 1, pfile);
		fwrite(MycRP.nameColSet, sizeof(char), MAX_COL_SETS * 64, pfile);
		fwrite(&MycRP.DrawColMode, sizeof(UINT32), 1, pfile);
		fwrite(&MycRP.Draw_Mode, sizeof(UINT8), 1, pfile);
		fwrite(&MycRP.Mask_Sel_Mode, sizeof(int), 1, pfile);
		fwrite(&MycRP.Fill_Mode, sizeof(BOOL), 1, pfile);
		fwrite(MycRP.Mask_Names, sizeof(char), MAX_MASKS * SIZE_MASK_NAME, pfile);
		fwrite(&MycRP.nSections, sizeof(UINT32), 1, pfile);
		fwrite(MycRP.Section_Firsts, sizeof(UINT32), MAX_SECTIONS, pfile);
		fwrite(MycRP.Section_Names, sizeof(char), MAX_SECTIONS * SIZE_SECTION_NAMES, pfile);
		fwrite(MycRP.Sprite_Names, sizeof(char), 255 * SIZE_SECTION_NAMES, pfile);
		fwrite(MycRP.Sprite_Col_From_Frame, sizeof(UINT32), 255, pfile);
		fwrite(MycRP.FrameDuration, sizeof(UINT32), nFrames, pfile);
		fwrite(MycRP.Sprite_Edit_Colors, 1, 16 * 255, pfile);
		fwrite(MycRP.SaveDir, 1, 260, pfile);
		fwrite(MycRP.SpriteRect, sizeof(UINT16), 4 * 255, pfile);
		fwrite(MycRP.SpriteRectMirror, sizeof(BOOL), 2 * 255, pfile);
		fclose(pfile);
		return true;
	}

	bool Load_cRP(char* name)
	{
		// cRP must be loaded after cROM
		//char tbuf[MAX_PATH];
		//sprintf_s(tbuf, MAX_PATH, "%s%s", DumpDir, name);
		FILE* pfile;
		if (fopen_s(&pfile, name, "rb") != 0)
		{
			return false;
		}
		Free_cRP();
		MycRP.oFrames = (UINT8*)malloc(nFrames * serumWidth * serumHeight);
		if (!MycRP.oFrames)
		{
			cprintf("Can't get the buffer in Load_cRP");
			Free_cRP(); // We free the buffers we got
			fclose(pfile);
			return false;
		}
		MycRP.FrameDuration = (UINT32*)malloc(nFrames * sizeof(UINT32));
		if (!MycRP.FrameDuration)
		{
			cprintf("Can't get the buffer in Load_cRP");
			Free_cRP(); // We free the buffers we got
			fclose(pfile);
			return false;
		}
		fread(MycRP.name, 1, 64, pfile);
		fread(MycRP.oFrames, 1, nFrames * serumWidth * serumHeight, pfile);
		fread(MycRP.activeColSet, sizeof(BOOL), MAX_COL_SETS, pfile);
		fread(MycRP.ColSets, sizeof(UINT8), MAX_COL_SETS * 16, pfile);
		fread(&MycRP.acColSet, sizeof(UINT8), 1, pfile);
		fread(&MycRP.preColSet, sizeof(UINT8), 1, pfile);
		fread(MycRP.nameColSet, sizeof(char), MAX_COL_SETS * 64, pfile);
		fread(&MycRP.DrawColMode, sizeof(UINT32), 1, pfile);
		if (MycRP.DrawColMode == 2) MycRP.DrawColMode = 0;
		fread(&MycRP.Draw_Mode, sizeof(UINT8), 1, pfile);
		fread(&MycRP.Mask_Sel_Mode, sizeof(int), 1, pfile);
		fread(&MycRP.Fill_Mode, sizeof(BOOL), 1, pfile);
		fread(MycRP.Mask_Names, sizeof(char), MAX_MASKS * SIZE_MASK_NAME, pfile);
		fread(&MycRP.nSections, sizeof(UINT32), 1, pfile);
		fread(MycRP.Section_Firsts, sizeof(UINT32), MAX_SECTIONS, pfile);
		fread(MycRP.Section_Names, sizeof(char), MAX_SECTIONS * SIZE_SECTION_NAMES, pfile);
		fread(MycRP.Sprite_Names, sizeof(char), 255 * SIZE_SECTION_NAMES, pfile);
		fread(MycRP.Sprite_Col_From_Frame, sizeof(UINT32), 255, pfile);
		for (UINT ti = 0; ti < nFrames; ti++) MycRP.FrameDuration[ti] = 0;
		fread(MycRP.FrameDuration, sizeof(UINT32), nFrames, pfile);
		fread(MycRP.Sprite_Edit_Colors, 1, 16 * 255, pfile);
		fread(MycRP.SaveDir, 1, 260, pfile);
		memset(MycRP.SpriteRect, 255, sizeof(UINT16) * 4 * 255);
		fread(MycRP.SpriteRect, sizeof(UINT16), 4 * 255, pfile);
		fread(MycRP.SpriteRectMirror, sizeof(BOOL), 2 * 255, pfile);
		fclose(pfile);
		/*
		char tbuf[MAX_PATH];
		sprintf_s(tbuf, MAX_PATH, "%s%s.ouf", DumpDir, MycRom.name);
		if (fopen_s(&pfile, tbuf, "rb") != 0)
		{
			AffLastError((char*)"Load_ouf:fopen_s");
			return false;
		}
		fread(&MycRom.nSprites, sizeof(UINT), 1, pfile);
		fread(MycRom.SpriteDescriptions, sizeof(UINT16), MycRom.nSprites * MAX_SPRITE_SIZE * MAX_SPRITE_SIZE, pfile);
		fread(MycRP.Sprite_Names, sizeof(char), 255 * SIZE_SECTION_NAMES, pfile);
		fread(MycRP.Sprite_Col_From_Frame, sizeof(UINT32), 255, pfile);
		fread(MycRP.Sprite_Edit_Colors, 1, 16 * 255, pfile);
		fclose(pfile);
		*/
		return true;
	}


	unsigned int readFrame() {
		char row[260] = { 0 };
		char tmp[4] = { 0 };
		unsigned int rowcount = 0;
		int len = 0;

		lasttick = tick;
		
		bool CRLF = true;
		gzgets(gzinfile, row, 260);
		len = strlen(row);

		if (len > ticksize){
			while (len > ticksize) {
				for (int kk = 0; kk < len; kk++) {
					sscanf(row + kk, "%1x", &tmp);
					row[kk] = tmp[0];
					if (row[kk] > 3)
						render16 = true;
				}
				if (len % 2 == 1)
					CRLF = false;
				if (CRLF) {
					memcpy(buffer + ((len - 2) * rowcount), row, len - 2);
					srcWidth = len-2;
				}
				else {
					memcpy(buffer + ((len - 1) * rowcount), row, len - 1);
					srcWidth = len - 1;
				}
				gzgets(gzinfile,row, 260);
				len = strlen(row);
				if (len > 2 && len <= ticksize) {
					sscanf(row, "%10x", &tick);
					rowcount = 0;
					gzgets(gzinfile, row, 260);
					len = strlen(row);
				}
				else {
					rowcount++;
				}
			}

			srcHeight = rowcount;

			gzgets(gzinfile,row, 260);
			len = strlen(row);
			if (len == ticksize){
				sscanf(row, "%10x", &tick);
				rowcount = 0;
			}
			
		}
	
		position = gztell(gzinfile);

		if (lasttick != 0)
			return (tick - lasttick);
		else
			return 0;
	}

	private: System::Void load_crz_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
		unsigned char	bufSize[4];

		openFileDialog1->Filter = "CROM|*.crz";

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			if (!hModule) {
#ifdef _WIN64
				hModule = LoadLibrary(L"serum64.dll");
#else
				hModule = LoadLibrary(L"serum.dll");
#endif

				if (!hModule) {
					MessageBox::Show("No serum library found");
				}

				Serum_Load = (Serum_Load_t)GetProcAddress(hModule, "Serum_Load");
				Serum_Dispose = (Serum_Dispose_t)GetProcAddress(hModule, "Serum_Dispose");
				Serum_IdentifyFrame = (Serum_IdentifyFrame_t)GetProcAddress(hModule, "Serum_IdentifyFrame");
				Serum_GetNFrames = (Serum_GetNFrames_t)GetProcAddress(hModule, "Serum_GetNFrames");

			}
			if (!Serum_Load || !Serum_Dispose || !Serum_IdentifyFrame || !Serum_GetNFrames) {
				MessageBox::Show("Serum driver functions not found");
			}
			else {
				unsigned int noOfColors = 0;
				unsigned int trigger = 0;
				char altColorPath[MAX_PATH] = {};
				char cRPname[MAX_PATH] = {};

				char* crzName = (char*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName).ToPointer();
				char* ptr = strrchr(crzName, '\\');
				memcpy(cRPname, crzName, strlen(crzName) - 3);
				strcat(cRPname,"cRP");
				memcpy(MycRP.name, ptr + 1, strlen(ptr) - 5);
				memcpy(altColorPath, crzName, strlen(crzName) - strlen(ptr));
				sprintf_s(MycRP.SaveDir, MAX_PATH, "%s\\", altColorPath);
				ptr = strrchr(altColorPath, '\\');
				*ptr = 0;

				if (Serum_Load(altColorPath, MycRP.name, &serumWidth, &serumHeight, &noOfColors, &trigger)) {
					nFrames = Serum_GetNFrames();
					colorized->Text = nFrames.ToString();
					detectedFrames = 0;
					skippedFrames = 0;
					if (!Load_cRP(cRPname)) {
						MycRP.oFrames = (UINT8*)malloc(nFrames * serumWidth * serumHeight);
						memset(MycRP.oFrames, 0, nFrames * serumWidth * serumHeight);
						MycRP.FrameDuration = (UINT32*)malloc(nFrames * sizeof(UINT32));
						memset(MycRP.FrameDuration, 0, nFrames * 4);
						status->Text = "crz loaded";
					}
					else {
						detectedFrames = 0;
						for (int i = 0; i < nFrames; i++) {
							if (MycRP.FrameDuration[i] != 0)
								detectedFrames++;
						}
						detected->Text = detectedFrames.ToString();
						status->Text = "cRZ+cRP loaded";
					}
					load_dump->Enabled = true;
				}
			}
		}
	}

	private: System::Void load_dump_Click(System::Object^  sender, System::EventArgs^  e) {
		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
		unsigned char bufSize[4] = {};

		openFileDialog1->Filter = "Text Dumps|*.txt.gz";

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			if (gzoutfile)
				gzclose(gzoutfile);
			if (gzinfile)
				gzclose(gzinfile);
			if (infile)
				fclose(infile);
			if (isOpen)
				Serum_Dispose();
			if (gzoutfile)
				gzclose(gzoutfile);

			srcfileName = (char*)Marshal::StringToHGlobalAnsi(openFileDialog1->FileName).ToPointer();
			fopen_s(&infile, srcfileName, "rb");
			if (infile == NULL)
			{
				MessageBox::Show("File Error");
			}

			status->Text = "analyzing dump";
			DoEvents();

			fseek(infile, -4L, SEEK_END);
			fread(&bufSize[0], sizeof(unsigned char), 4, infile);
			fileSize = (unsigned int)((bufSize[3] << 24) | (bufSize[2] << 16) | (bufSize[1] << 8) | bufSize[0]);
			position = fileSize;
			fclose(infile);
			gzinfile = gzopen(srcfileName, "r");

			int len;
			char row[260] = {};

			gzgets(gzinfile, row, 16);
			len = strlen(row);
			if (len <= ticksize) {
				sscanf(row, "%10x", &tick);
			}
			ticksize = gztell(gzinfile);
			duration = readFrame();
			framesize = position;
			nSrcFrames = fileSize / framesize;
		}
		for (int i = 1; i < nSrcFrames; i++) {
			int frame = -1;
			frame = Serum_IdentifyFrame(buffer);
			if (frame >= 0 && MycRP.FrameDuration[frame] <= 0) {
				MycRP.FrameDuration[frame] = duration;
				memcpy (MycRP.oFrames + (serumWidth * serumHeight * frame), buffer, (serumWidth * serumHeight));
				detectedFrames++;
				detected->Text = detectedFrames.ToString();
				DoEvents();
			}
			else if (frame == -1) {
				skippedFrames++;
				skipped->Text = skippedFrames.ToString();
				DoEvents();
			}
			duration = readFrame();
		}
		Save_cRP(false);
		status->Text = "cRP saved";
	}

private: System::Void MyForm_Closed (System::Object^  sender, System::EventArgs^  e) {
	if (isOpen)
		Serum_Dispose();
	if (hModule)
		FreeLibrary(hModule);
	if (infile)
		fclose(infile);
	if (gzinfile)
		gzclose(gzinfile);
	if (gzoutfile)
		gzclose(gzoutfile);
	if (srcfileName)
		Marshal::FreeHGlobal((IntPtr)srcfileName);
	
}

private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {

}
};
}
