/********************************************************************
**	file: 		PlatoExporter.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-3-8
**	purpose:	
*********************************************************************/
#ifndef __PlatoExporter_h__
#define __PlatoExporter_h__

#include "PlatoCommon.h"
#include "PlatoTreeEnumProc.h"

class PlatoExporter : public SceneExport
{
public:
	virtual int				ExtCount();
	
	virtual const MCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")

	virtual const MCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")

	virtual const MCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")

	virtual const MCHAR *	AuthorName();				// ASCII Author name

	virtual const MCHAR *	CopyrightMessage();			// ASCII Copyright message

	virtual const MCHAR *	OtherMessage1();			// Other message #1

	virtual const MCHAR *	OtherMessage2();			// Other message #2

	virtual unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)

	virtual void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

	virtual int				DoExport(const MCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);	// Export file

	virtual BOOL SupportsOptions(int ext, DWORD options); 

public:
	static const std::string tName;
	static const std::string tLongDesc;
	static const std::string tShortDesc;
	static const std::string tAuthor;
	static const std::string tCopyRight;
	static const std::string tCategory;
	static const int tVersion;
	// class ID
	static const Class_ID tCID;
	//// SuperClass ID
	static const SClass_ID tSID;
/*	static HINSTANCE	tInstance;*/

private:
	std::vector<std::string>	_exts;
	PlatoTreeEnumProc	_enum;
public:
	PlatoExporter();
	virtual ~PlatoExporter();

private:
	friend INT_PTR CALLBACK ExportOptionsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

static INT_PTR CALLBACK ExportOptionsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static PlatoExporter* exp;

	switch(message) {
		case WM_INITDIALOG:
			SetWindowContextHelpId(hDlg, idh_3dsexp_export);
			exp = (PlatoExporter*) lParam;
			CenterWindow(hDlg,GetParent(hDlg));
			SetFocus(hDlg);
			CheckDlgButton(hDlg, IDC_CHECK_EXPORT_ANIMATION, 0);
			CheckDlgButton(hDlg, IDC_CHECK_EXPORT_VERTEX_COLOR, 0);
			return FALSE;

		case WM_DESTROY:
			return FALSE;

		case WM_COMMAND:
			{
				switch(LOWORD(wParam)) 
				{
				case IDOK:
					GetExportOptions()->exportAnims = IsDlgButtonChecked(hDlg, IDC_CHECK_EXPORT_ANIMATION);
					GetExportOptions()->exportVertexColour = IsDlgButtonChecked(hDlg, IDC_CHECK_EXPORT_VERTEX_COLOR);
					GetExportOptions()->deleteUnReferrencedBones = IsDlgButtonChecked(hDlg, IDC_DELETE_UNREFERRENCED_BONES);

					EndDialog(hDlg, 1);
					return TRUE;

				case IDCANCEL:
					EndDialog(hDlg, 0);
					return FALSE;
				}
			}break;
			
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_CONTEXTHELP) 
			{
				DoHelp(HELP_CONTEXT, idh_3dsexp_export);
				return FALSE;
			}
	}
	return FALSE;
}
#endif // __PlatoExporter_h__ 
