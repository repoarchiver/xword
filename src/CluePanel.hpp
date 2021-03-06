// This file is part of XWord
// Copyright (C) 2011 Mike Richards ( mrichards42@gmx.com )
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#ifndef MY_CLUE_PANEL_H
#define MY_CLUE_PANEL_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#    include <wx/panel.h>
#    include <wx/stattext.h>
#endif

#include "ClueListBox.hpp"
#include "utils/string.hpp" // puz / wx string conversions


extern const wxChar* CluePanelNameStr;
class MyFrame;

class CluePanel
    : public wxPanel
{
    friend class MyFrame;
public:
    // Enums
    //------

    // Color type
    enum textColor
    {
        TEXT,
        BACKGROUND
    };

    // Focused direction
    enum focusDirection
    {
        CROSSING,
        FOCUSED
    };

public:
    CluePanel() { Init(); }

    CluePanel(wxWindow * parent,
              wxWindowID id,
              const wxString & heading = wxEmptyString,
              const wxPoint & pos = wxDefaultPosition,
              const wxSize & size = wxDefaultSize,
              long style = wxBORDER_NONE,
              const wxString & name = CluePanelNameStr)
    {
        Init();
        Create(parent, id, heading, pos, size, style, name);
    }

    virtual ~CluePanel();


    bool Create(wxWindow * parent,
                wxWindowID id,
                const wxString & heading = wxEmptyString,
                const wxPoint & pos = wxDefaultPosition,
                const wxSize & size = wxDefaultSize,
                long style = wxBORDER_NONE,
                const wxString & name = CluePanelNameStr);

    // Access to the ClueListBox
    //--------------------------
    void SetClueList(puz::ClueList * clues)
        { m_clueList->SetClueList(clues); }

    void ClearClueList() { m_clueList->ClearClueList(); }

    puz::Clue * GetClue() { return m_clueList->GetClue(); }

    bool SetClue(const puz::Clue * clue, focusDirection focus)
    {
        if (clue == GetClue() && focus == m_focusDirection)
            return true;
        if (m_focusDirection != focus)
        {
            m_clueList->SetSelectionForeground(m_colors[focus][TEXT]);
            m_clueList->SetSelectionBackground(m_colors[focus][BACKGROUND]);
        }
        m_focusDirection = focus;
        return m_clueList->SetClue(clue);
    }

    focusDirection GetDirection() const { return m_focusDirection; }


    // List colors
    //------------

    // This works well from the outside, but is a mess internally.
    // It would be nice to just have access to m_clueList directly so that we
    // don't need all these extra functions to access it, but without these
    // functions it's difficult to manage the color swapping when the user
    // changes from an across clue to a down clue.  It's definitely possible
    // to implement, but this setup already exists and I don't feel like
    // taking the time to mess with it.

    void SetSelectionForeground(const wxColor & color)
    {
        m_colors[FOCUSED][TEXT] = color;
        if (m_focusDirection == FOCUSED)
            m_clueList->SetSelectionForeground(color) ;
    }

    void SetSelectionBackground(const wxColor & color)
    {
        m_colors[FOCUSED][BACKGROUND] = color;
        if (m_focusDirection == FOCUSED)
            m_clueList->SetSelectionBackground(color) ;
    }

    void SetCrossingForeground(const wxColor & color)
    {
        m_colors[CROSSING][TEXT] = color;
        if (m_focusDirection == CROSSING)
            m_clueList->SetSelectionForeground(color) ;
    }

    void SetCrossingBackground(const wxColor & color)
    {
        m_colors[CROSSING][BACKGROUND] = color;
        if (m_focusDirection == CROSSING)
            m_clueList->SetSelectionBackground(color) ;
    }

    const wxColour & GetSelectionForeground() const
        { return m_colors[FOCUSED][TEXT]; }
    const wxColour & GetSelectionBackground() const
        { return m_colors[FOCUSED][BACKGROUND]; }
    const wxColour & GetCrossingForeground() const
        { return m_colors[CROSSING][TEXT]; }
    const wxColour & GetCrossingBackground() const
        { return m_colors[CROSSING][BACKGROUND]; }

    bool SetForegroundColour(const wxColor & color)
        { return m_clueList->SetForegroundColour(color); }
    bool SetBackgroundColour(const wxColor & color)
        { return m_clueList->SetBackgroundColour(color); }

    wxColour GetForegroundColour() const
        { return m_clueList->GetForegroundColour(); }
    wxColour GetBackgroundColour() const
        { return m_clueList->GetBackgroundColour(); }


    // Clue list font
    //---------------

    // This function is a mess, because we need to finish creating the underlying
    // wxPanel before we can create child windows, but the wxMac implementation
    // calls SetFont while the wxPanel is being created . . .
    bool SetFont(const wxFont & font)
    {
        bool ret = wxPanel::SetFont(font);
        if (! ret)
            return false;
        if (m_clueList != NULL)
            ret &= m_clueList->SetFont(font);
        return ret;
    }


    // Clue list margins
    //------------------
    void SetMargins(const wxPoint & margins) 
        { m_clueList->SetMargins(margins); }
    wxPoint GetMargins() const
        { return m_clueList->GetMargins(); }


    // Clue list heading
    //------------------
    void SetHeading(const wxString & heading) { m_heading->SetLabel(heading); }
    wxString GetHeading() const { return m_heading->GetLabel(); }
    bool SetHeadingFont(const wxFont & font)
    {
        if (! m_heading->SetFont(font))
            return false;
        Layout();
        m_heading->Refresh();
        return true;
    }
    wxFont GetHeadingFont() const
        { return m_heading->GetFont(); }

    bool SetHeadingForeground(const wxColor & color)
    {
        const bool ret = m_heading->SetForegroundColour(color);
        m_heading->Refresh();
        return ret;
    }
    bool SetHeadingBackground(const wxColor & color)
    {
        const bool ret = m_heading->SetBackgroundColour(color);
        m_heading->Refresh();
        return ret;
    }

    wxColour GetHeadingForeground() const
        { return m_heading->GetForegroundColour(); }
    wxColour GetHeadingBackground() const
        { return m_heading->GetBackgroundColour(); }

private:
    void Init() { m_heading = NULL; m_clueList = NULL; }

    wxStaticText * m_heading;
    ClueListBox * m_clueList;
    focusDirection m_focusDirection;

    // [ Focused / unfocused ][ text / background ]
    wxColor m_colors[2][2];

    // Check to see if the panel is very small
    // If it is, display the clue box next to the heading
    void OnSize(wxSizeEvent & evt);

    void OnClueSelect(wxCommandEvent & WXUNUSED(evt));

    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(CluePanel)
    DECLARE_DYNAMIC_CLASS(CluePanel)
};


#endif // MY_CLUE_PANEL_H