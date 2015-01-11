/*! \file linenumberarea.h
	\author HiddenSeeker

	A part of code editor, which displays line numbers,  based on both examples of rich editor and other
	http://qt-project.org/doc/qt-4.8/widgets-codeeditor.html ,
	http://qt-project.org/doc/qt-4.8/richtext-syntaxhighlighter.html
 */
#pragma once
#include "../textedit/textedit.h"

#include <QPaintEvent>

class QCompleter;

namespace gui
{

namespace codeedit
{
class CodeEdit;
/*! A part of code editor, which displays line numbers based on both examples of rich editor and other
	http://qt-project.org/doc/qt-4.8/widgets-codeeditor.html ,
	http://qt-project.org/doc/qt-4.8/richtext-syntaxhighlighter.html
 */
class LineNumberArea : public QWidget
{
public:
	/*! Constructs a text edit widget
		\param[in] editor an editor
	 */ 
	LineNumberArea(gui::codeedit::CodeEdit* editor);
     /*! Destructor for widget
	  */
	virtual ~LineNumberArea();
	/*! Returns size hint for line number
		\return size hint
	 */
	QSize sizeHint() const;
protected:
	/*! A rendering event for widget
		\param[in] e event
	 */
	void paintEvent(QPaintEvent* e);
	/*! A code editor
	 */
	gui::codeedit::CodeEdit* m_code_editor;
};

}

}
