#include <wx/wx.h>

class MPFracApp : public wxApp {
public:
    virtual bool OnInit();
};

class MPFracFrame : public wxFrame {
    wxString _params;
    wxString _rawIn;
    wxString _rawOut;
    wxString _imageOut;
    unsigned long _prec;
    unsigned long _threads;

    wxTextCtrl *_redirectBox;

    void OnSetThreads(wxCommandEvent &event);
    void OnSetPrecision(wxCommandEvent &event);
    void OnSetParams(wxCommandEvent &event);
    void OnSetRawOut(wxCommandEvent &event);
    void OnSetRawIn(wxCommandEvent &event);
    void OnSetImageOut(wxCommandEvent &event);

    void OnRun(wxCommandEvent &event);

    void OnExit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
public:
    MPFracFrame(const wxString& title, const wxPoint& pos, const wxSize & size);

};

enum {
    ID_SetThreads = 1,
    ID_SetPrecision,
    ID_SetParams,
    ID_SetRawOut,
    ID_SetRawIn,
    ID_SetImageOut,
    ID_Run
};

wxBEGIN_EVENT_TABLE(MPFracFrame, wxFrame)
    EVT_MENU(ID_SetThreads, MPFracFrame::OnSetThreads)
    EVT_MENU(ID_SetPrecision, MPFracFrame::OnSetPrecision)
    EVT_MENU(ID_SetParams, MPFracFrame::OnSetParams)
    EVT_MENU(ID_SetRawOut, MPFracFrame::OnSetRawOut)
    EVT_MENU(ID_SetRawIn, MPFracFrame::OnSetRawIn)
    EVT_MENU(ID_SetImageOut, MPFracFrame::OnSetImageOut)
    EVT_MENU(ID_Run, MPFracFrame::OnRun)
    EVT_MENU(wxID_EXIT, MPFracFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MPFracFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MPFracApp);

bool MPFracApp::OnInit() {
    MPFracFrame *mpFracFrame = new MPFracFrame("mpfrac-gui", wxPoint(50, 50), wxSize(640, 480));
    mpFracFrame->Show(true);

    return true;
}

MPFracFrame::MPFracFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
 : wxFrame(NULL, wxID_ANY, title, pos, size) {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_SetParams, "&Set parameters...\tCtrl-F", "Set the parameters file for the fractal");
    menuFile->AppendSeparator();
    menuFile->Append(ID_SetRawIn, "&Set raw input...\tCtrl-I", "Set the input raw fractal data file");
    menuFile->Append(ID_SetRawOut, "&Set raw output...\tCtrl-R", "Set the output raw fractal data file");
    menuFile->AppendSeparator();
    menuFile->Append(ID_SetImageOut, "&Set image output...\tCtrl-O", "Set the output PNG image file");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuSettings = new wxMenu;
    menuSettings->Append(ID_SetPrecision, "&Set precision...\tCtrl-P", "Set the floating point precision in bits");
    menuSettings->Append(ID_SetThreads, "&Set threads...\tCtrl-T", "Set the number of threads for fractal calculation");

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenu *menuRun = new wxMenu;
    menuRun->Append(ID_Run, "&Run\tCtrl-X", "Run mpfrac");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuSettings, "&Settings");
    menuBar->Append(menuRun, "&Run");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    _redirectBox = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1), wxSize(250, 150));

    Centre();

    CreateStatusBar();
    SetStatusText("mpfrac-gui v0.1");
}

void MPFracFrame::OnSetThreads(wxCommandEvent &event) {
    wxTextEntryDialog threadsDialog(this, _("Set thread count:"));

    if(threadsDialog.ShowModal() == wxID_CANCEL)
        return;

    if(!threadsDialog.GetValue().ToULong(&_threads)) {
        wxMessageDialog(this, _("Invalid thread count!"), _("Error"), wxOK | wxICON_ERROR | wxCENTRE).ShowModal();
    }
}

void MPFracFrame::OnSetPrecision(wxCommandEvent &event) {
    wxTextEntryDialog precDialog(this, _("Set precision (bits):"));

    if(precDialog.ShowModal() == wxID_CANCEL)
        return;

    if(!precDialog.GetValue().ToULong(&_prec)) {
        wxMessageDialog(this, _("Invalid precision!"), _("Error"), wxOK | wxICON_ERROR | wxCENTRE).ShowModal();
    }
}

void MPFracFrame::OnSetParams(wxCommandEvent &event) {
    wxFileDialog paramsDialog(this, _("Set parameter file"), "", "", "All files|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(paramsDialog.ShowModal() == wxID_CANCEL)
        return;

    _params = paramsDialog.GetPath();
}

void MPFracFrame::OnSetRawOut(wxCommandEvent &event) {
    wxFileDialog rawOutDialog(this, _("Set image output file"), "", "", "All files|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(rawOutDialog.ShowModal() == wxID_CANCEL)
        return;

    _rawOut = rawOutDialog.GetPath();
}

void MPFracFrame::OnSetRawIn(wxCommandEvent &event) {
    wxFileDialog rawInDialog(this, _("Set raw input file"), "", "", "All files|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(rawInDialog.ShowModal() == wxID_CANCEL)
        return;

    _rawIn = rawInDialog.GetPath();
}

void MPFracFrame::OnSetImageOut(wxCommandEvent &event) {
    wxFileDialog imageOutDialog(this, _("Set image output file"), "", "", "All files|*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(imageOutDialog.ShowModal() == wxID_CANCEL)
        return;

    _imageOut = imageOutDialog.GetPath();
}

void MPFracFrame::OnRun(wxCommandEvent &event) {
    wxProcess *mpfrac = new wxProcess(wxPROCESS_REDIRECT); 
    wxExecute(_("./mpfrac -R -I\"") + _rawIn + _("\" -t") + _(std::to_string(_threads) + _(" -p") + _(std::to_string(_prec)) + _(" -o\"") + _imageOut) + _("\""), );

    wxString msg;
    wxInputStream *p_out = mpfrac->GetInputStream();
    wxTextInputStream tStream(*p_out);
    msg = tStream.ReadLine();
    _redirectBox->AppendText(msg+wxT("\n"));

    wxShell();
}

void MPFracFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void MPFracFrame::OnAbout(wxCommandEvent &event) {
    wxMessageBox("mpfrac-gui version 0.1\n[altanh@uw.edu]", "About mpfrac-gui", wxOK | wxICON_INFORMATION);
}
