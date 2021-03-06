#define __STL_CONFIG_H
#include <vdr/menu.h>
#include "displayaudiotracksview.h"

cDisplayAudiotracksView::cDisplayAudiotracksView(int numTracks, cTemplateView *tmplView) : cView(tmplView) {
    DeleteOsdOnExit();
    SetFadeTime(tmplView->GetNumericParameter(ptFadeTime));

    this->numTracks = numTracks;

    cTemplateViewList *tmplMenuItems = tmplView->GetViewList(vlMenuItem);
    listView = NULL;
    if (tmplMenuItems) {
        listView = new cDisplayMenuListView(tmplMenuItems, numTracks);
    }
}

cDisplayAudiotracksView::~cDisplayAudiotracksView() {
    if (listView)
        delete listView;
    CancelSave();
    FadeOut();
}

bool cDisplayAudiotracksView::createOsd(void) {
    cRect osdSize = tmplView->GetOsdSize();
    bool ok = CreateOsd(cOsd::OsdLeft() + osdSize.X(),
                        cOsd::OsdTop() + osdSize.Y(),
                        osdSize.Width(),
                        osdSize.Height());
    return ok;
}

void cDisplayAudiotracksView::DrawBackground(void) {
    if (!ViewElementImplemented(veBackground)) {
        return;
    }

    map < string, string > stringTokens;
    map < string, int > intTokens;

    intTokens.insert(pair<string,int>("numtracks", numTracks));

    DrawViewElement(veBackground, &stringTokens, &intTokens);
}

void cDisplayAudiotracksView::DrawHeader(const char *title, int audioChannel) {
    if (!ViewElementImplemented(veHeader)) {
        return;
    }

    map < string, string > stringTokens;
    map < string, int > intTokens;

    intTokens.insert(pair<string,int>("numtracks", numTracks));
    if (audioChannel < 0) {
        intTokens.insert(pair<string,int>("isac3", true));
        intTokens.insert(pair<string,int>("isstereo", false));
    } else {
        intTokens.insert(pair<string,int>("isac3", false));
        intTokens.insert(pair<string,int>("isstereo", true));
    }
    stringTokens.insert(pair<string,string>("title", title));

    ClearViewElement(veHeader);
    DrawViewElement(veHeader, &stringTokens, &intTokens);
}

void cDisplayAudiotracksView::RenderMenuItems(void) {
    if (listView)
        listView->Render();
}

void cDisplayAudiotracksView::Action(void) {
    SetInitFinished();
    FadeIn();
    DoFlush();
    cView::Action();
}
