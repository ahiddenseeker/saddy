######################################################################
# Automatically generated by qmake (2.01a) ?? ????. 14 17:12:28 2013
######################################################################

QT += core gui opengl script
CONFIG += console opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              ifaceed \
              ifaceed/core \
              ifaceed/editorcore \
              ifaceed/gui \
              ifaceed/history \
              ifaceed/objects \
              ifaceed/unittests \
              ifaceed/core/states \
              ifaceed/gui/spritewidget
INCLUDEPATH += . \
               ifaceed \
               ifaceed/gui/spritewidget \
               ifaceed/core \
               ifaceed/editorcore \
               ifaceed/gui \
               ifaceed/history \
               ifaceed/unittests \
               ifaceed/objects \
               ifaceed/core/states \
	       ../../include/ \
	       ../../include/config/ \
	       ../../include/log/ \
	       ../../include/marshal/ \
	       ../../include/os/ \
	       ../../include/primitives/ \
	       ../../include/templates/ \

# Input
HEADERS += ifaceed/mainpanel.h \
    ifaceed/core/typeconverters/load.h \
    ifaceed/core/typeconverters/qcolortosadacolor.h \
    ifaceed/core/typeconverters/qcolortosadcolor.h \
    ifaceed/core/typeconverters/qlistqlistqcolortosadvectorsadvectoracolor.h \
    ifaceed/core/typeconverters/qstringtosadstring.h \
    ifaceed/core/typeconverters/sadacolortoqcolor.h \
    ifaceed/core/typeconverters/sadcolortoqcolor.h \
    ifaceed/core/typeconverters/sadstringtoqstring.h \
    ifaceed/core/typeconverters/sadvectorsadvectoracolortoqlistqlistqcolor.h \
    ifaceed/core/typeconverters/save.h \
    ifaceed/core/editor.h \
    ifaceed/core/qttarget.h \
    ifaceed/core/quitreason.h \
    ifaceed/core/saddythread.h \
    ifaceed/core/shared.h \
    ifaceed/acolordialog.h \
    ifaceed/macro.h \
    ifaceed/qwwlongspinbox.h \
    ifaceed/qwwulongspinbox.h \
    ifaceed/wwglobal.h \
    ifaceed/wwglobal_p.h \
    ifaceed/history/command.h \
    ifaceed/history/history.h \
    ifaceed/history/database/newproperty.h \
    ifaceed/history/database/removeproperty.h \
    ifaceed/gui/anglewidget/anglewidget.h \
    ifaceed/gui/colorpicker/colorpicker.h \
    ifaceed/gui/colorview/colorview.h \
    ifaceed/gui/fontsizewidget/fontsizewidget.h \
    ifaceed/gui/rectwidget/rectwidget.h \
    ifaceed/gui/resourcetreewidget/cell.h \
    ifaceed/gui/resourcetreewidget/celldelegate.h \
    ifaceed/gui/resourcetreewidget/defaultimage.h \
    ifaceed/gui/resourcetreewidget/resourcecache.h \
    ifaceed/gui/resourcetreewidget/resourcetreewidget.h \
    ifaceed/gui/table/delegate.h \
    ifaceed/gui/table/delegatefactory.h \
    ifaceed/gui/table/doubledelegate.h \
    ifaceed/gui/table/floatdelegate.h \
    ifaceed/gui/table/booldelegate.h \
    ifaceed/gui/table/intdelegate.h \
    ifaceed/gui/table/longdelegate.h \
    ifaceed/gui/table/longlongdelegate.h \
    ifaceed/gui/table/sadacolordelegate.h \
    ifaceed/gui/table/sadcolordelegate.h \
    ifaceed/gui/table/sadstringdelegate.h \
    ifaceed/gui/table/shortdelegate.h \
    ifaceed/gui/table/signedchardelegate.h \
    ifaceed/gui/table/unsignedchardelegate.h \
    ifaceed/gui/table/unsignedintdelegate.h \
    ifaceed/gui/table/unsignedlongdelegate.h \
    ifaceed/gui/table/unsignedlonglongdelegate.h \
    ifaceed/gui/table/unsignedshortdelegate.h \
    ifaceed/gui/tuplewidget/doubletuplewidget.h \
    ifaceed/gui/tuplewidget/int64tuplewidget.h \
    ifaceed/gui/table/sadpoint2ddelegate.h \
    ifaceed/gui/table/sadsize2ddelegate.h \
    ifaceed/gui/table/sadpoint2idelegate.h \
    ifaceed/gui/table/sadsize2idelegate.h \
    ifaceed/gui/eventfilter.h \
    ifaceed/history/scenes/scenesadd.h \
    ifaceed/history/scenes/sceneschangename.h \
    ifaceed/history/scenes/sceneslayerswap.h \
    ifaceed/history/scenes/scenesremove.h \
    ifaceed/core/synchronization.h \
    ifaceed/history/scenenodes/scenenodesnew.h \
    ifaceed/gui/labelactions.h \
    ifaceed/gui/scenenodeactions.h \
    ifaceed/core/borders/activeborder.h \
    ifaceed/core/borders/border.h \
    ifaceed/core/borders/selectionborder.h \
    ifaceed/history/label/changefontname.h \
    ifaceed/closuremethodcall.h \
    ifaceed/blockedclosuremethodcall.h \
    ifaceed/history/label/changefontsize.h \
    ifaceed/history/label/changetext.h \
    ifaceed/history/label/changelinespacing.h \
    ifaceed/history/scenenodes/scenenodeschangename.h \
    ifaceed/core/typeconverters/sadrect2dtoqrectf.h \
    ifaceed/core/typeconverters/qrectftosadrect2d.h \
    ifaceed/history/scenenodes/scenenodeschangearea.h \
    ifaceed/history/database/changeproperty.h \
    ifaceed/history/scenenodes/scenenodeschangeangle.h \
    ifaceed/history/scenenodes/scenenodeschangecolor.h \
    ifaceed/history/scenenodes/scenenodeschangeproperty.h \
    ifaceed/history/scenenodes/scenenodeschangepropertywhichlinkedtocheckbox.h \
    ifaceed/history/scenenodes/scenenodeschangevisibility.h \
    ifaceed/gui/rotationprocess.h \
    ifaceed/gui/sprite2dactions.h \
    ifaceed/history/sprite2d/changeflipx.h \
    ifaceed/history/sprite2d/changeflipy.h \
    ifaceed/history/sprite2d/changeoptions.h \
    ifaceed/history/sprite2d/makebackground.h \
    ifaceed/history/scenenodes/scenenodesremove.h \
    ifaceed/history/scenenodes/scenenodeslayerswap.h \
    ifaceed/history/scenes/scenesclear.h \
    ifaceed/history/customobject/customobjectchangeproperty.h \
    ifaceed/gui/customobjectactions.h \
    ifaceed/history/customobject/customobjectchangeschema.h \
    ifaceed/gui/updateelement.h \
    ifaceed/core/borders/deletehotspot.h \
    ifaceed/core/borders/resizehotspot.h \
    ifaceed/core/selection.h \
    ifaceed/reloadfilelist.h \
    ifaceed/gui/renderways.h \
    ifaceed/gui/wayactions.h \
    ifaceed/history/ways/waysnew.h \
    ifaceed/history/ways/waysremove.h \
    ifaceed/history/ways/wayschangeproperty.h \
    ifaceed/history/ways/wayschangename.h \
    ifaceed/history/ways/wayschangetotaltime.h \
    ifaceed/history/ways/wayschangeclosed.h \
    ifaceed/history/ways/wayswaypointnew.h \
    ifaceed/history/ways/wayswaypointchange.h \
    ifaceed/history/ways/wayswaypointremove.h \
    ifaceed/history/ways/wayswaypointswap.h \
    ifaceed/gui/dialogueactions.h \
    ifaceed/history/dialogues/dialoguesremove.h \
    ifaceed/history/dialogues/dialogueschangename.h \
    ifaceed/history/dialogues/dialoguesnew.h \
    ifaceed/history/dialogues/dialoguesphrasechangeduration.h \
    ifaceed/history/dialogues/dialoguesphrasechangelineeditbasedproperty.h \
    ifaceed/history/dialogues/dialoguesphrasechangephrase.h \
    ifaceed/history/dialogues/dialoguesphrasenew.h \
    ifaceed/history/dialogues/dialoguesphraseremove.h \
    ifaceed/history/dialogues/dialoguesphraseswap.h \
    ifaceed/history/animations/animationsaddtocomposite.h \
    ifaceed/history/animations/animationschangeblinkingfrequency.h \
    ifaceed/history/animations/animationschangecameraangle.h \
    ifaceed/history/animations/animationschangecameraoffset.h \
    ifaceed/history/animations/animationschangecamerapivot.h \
    ifaceed/history/animations/animationschangecolorcolor.h \
    ifaceed/history/animations/animationschangefontlistfonts.h \
    ifaceed/history/animations/animationschangefontsizesize.h \
    ifaceed/history/animations/animationschangelist.h \
    ifaceed/history/animations/animationschangelooped.h \
    ifaceed/history/animations/animationschangename.h \
    ifaceed/history/animations/animationschangeproperty.h \
    ifaceed/history/animations/animationschangerect.h \
    ifaceed/history/animations/animationschangerotateangle.h \
    ifaceed/history/animations/animationschangeshakingfrequency.h \
    ifaceed/history/animations/animationschangetime.h \
    ifaceed/history/animations/animationschangewaymovingway.h \
    ifaceed/history/animations/animationsnew.h \
    ifaceed/history/animations/animationsremove.h \
    ifaceed/history/animations/animationsremovefromcomposite.h \
    ifaceed/history/animations/animationsswapincomposite.h \
    ifaceed/history/groups/groupsaddinstance.h \
    ifaceed/history/groups/groupschangelooped.h \
    ifaceed/history/groups/groupschangename.h \
    ifaceed/history/groups/groupschangeproperty.h \
    ifaceed/history/groups/groupsnew.h \
    ifaceed/history/groups/groupsremove.h \
    ifaceed/history/groups/groupsremoveinstance.h \
    ifaceed/history/instances/instanceschangeanimation.h \
    ifaceed/history/instances/instanceschangename.h \
    ifaceed/history/instances/instanceschangeobject.h \
    ifaceed/history/instances/instanceschangeproperty.h \
    ifaceed/history/instances/instanceschangestarttime.h \
    ifaceed/history/instances/instanceschangeway.h \
    ifaceed/history/instances/instancesnew.h \
    ifaceed/history/instances/instancesremove.h \
    ifaceed/history/batchcommand.h \
    ifaceed/gui/codeedit/codeedit.h \
    ifaceed/gui/codeedit/highlighter.h \
    ifaceed/gui/codeedit/linenumberarea.h \
    ifaceed/gui/textedit/textedit.h \
    ifaceed/gui/animationactions.h \
    ifaceed/gui/animationgroupprocess.h \
    ifaceed/gui/animationinstanceprocess.h \
    ifaceed/gui/animationprocess.h \
    ifaceed/gui/groupactions.h \
    ifaceed/gui/instanceactions.h \
    ifaceed/scripting/scripting.h \
    ifaceed/scripting/tovalue.h \
    ifaceed/scripting/callable.h \
    ifaceed/scripting/constructorcall.h \
    ifaceed/scripting/fromvalue.h \
    ifaceed/scripting/functioncall.h \
    ifaceed/scripting/makeconstructor.h \
    ifaceed/scripting/makefunctioncall.h \
    ifaceed/scripting/multimethod.h \
    ifaceed/scripting/queryobject.h \
    ifaceed/scripting/registerconstructor.h \
    ifaceed/scripting/scriptinglog.h \
    ifaceed/scripting/classwrapper.h \
    ifaceed/scripting/point2d.h \
    ifaceed/scripting/point2i.h \
    ifaceed/scripting/point3d.h \
    ifaceed/scripting/point3i.h \
    ifaceed/scripting/rect2d.h \
    ifaceed/scripting/rect2i.h \
    ifaceed/scripting/acolor.h \
    ifaceed/scripting/color.h \
    ifaceed/scripting/size2d.h \
    ifaceed/scripting/size2i.h \
    ifaceed/scripting/scriptingcall.h \
    ifaceed/scripting/makescriptingcall.h \
    ifaceed/scripting/scenes/scenesnamesetter.h \
    ifaceed/scripting/abstractgetter.h \
    ifaceed/scripting/abstractsetter.h \
    ifaceed/scripting/scenes/scenesbindings.h \
    ifaceed/scripting/database/databasebindings.h \
    ifaceed/scripting/database/databasepropertysetter.h \
    ifaceed/scripting/database/databasepropertygetter.h \
    ifaceed/scripting/querytable.h \
    ifaceed/scripting/scenenodes/scenenodesbindings.h \
    ifaceed/scripting/queryresource.h \
    ifaceed/scripting/scenenodes/scenenodesabstractsetter.h \
    ifaceed/scripting/scenenodes/scenenodessetter.h \
    ifaceed/scripting/scenenodes/scenenodesareasetter.h \
    ifaceed/scripting/scenenodes/scenenodesfontsizesetter.h \
    ifaceed/scripting/scenenodes/scenenodesfontsetter.h \
    ifaceed/scripting/scenenodes/scenenodescustomgetter.h \
    ifaceed/scripting/scenenodes/scenenodescustomsetter.h \
    ifaceed/scripting/scenenodes/scenenodesflagsetter.h \
    ifaceed/scripting/scenenodes/scenenodesoptionssetter.h \
    ifaceed/scripting/scenenodes/scenenodesschemasetter.h \
    ifaceed/scripting/ways/waysbindings.h \
    ifaceed/scripting/animations/animationsbindings.h \
    ifaceed/scripting/animations/animationssetter.h \
    ifaceed/scripting/animations/animationswidgetsetter.h \
    ifaceed/scripting/dialogues/dialoguesbindings.h \
    ifaceed/scripting/dialogues/dialoguesphraseref.h \
    ifaceed/scripting/dialogues/dialoguessetter.h \
    ifaceed/scripting/ways/wayspointref.h \
    ifaceed/scripting/ways/wayssetter.h \
    ifaceed/scripting/animations/animationswaysetter.h \
    ifaceed/gui/codeedit/completer.h \
    ifaceed/scripting/abstractcondition.h \
    ifaceed/scripting/abstracttovalue.h \
    ifaceed/scripting/isaabb.h \
    ifaceed/scripting/instances/instancesbindings.h \
    ifaceed/scriptinghelp.h \
    ifaceed/scripting/groups/groupsbindings.h \
    ifaceed/scripting/groups/groupsloopedsetter.h \
    ifaceed/scripting/groups/groupsnamesetter.h \
    ifaceed/scripting/instances/instancesanimationdbsetter.h \
    ifaceed/scripting/instances/instancesanimationsetter.h \
    ifaceed/scripting/instances/instancesnamesetter.h \
    ifaceed/scripting/instances/instancesobjectsetter.h \
    ifaceed/scripting/instances/instancesstarttimesetter.h \
    ifaceed/scripting/instances/instanceswaysetter.h \
    ifaceed/history/animations/animationschangepropertyaspoint2displayedintwospinboxes.h \
    ifaceed/history/animations/animationschangeresizeendingsize.h \
    ifaceed/history/animations/animationschangeresizestartingsize.h \
    ifaceed/scripting/animations/animationspoint2dsetter.h
FORMS += ifaceed/mainpanel.ui \
    ifaceed/acolordialog.ui \
    ifaceed/reloadfilelist.ui \
    ifaceed/scriptinghelp.ui
SOURCES += ifaceed/main.cpp \
           ifaceed/mainpanel.cpp \
    ifaceed/core/typeconverters/qcolortosadacolor.cpp \
    ifaceed/core/typeconverters/qcolortosadcolor.cpp \
    ifaceed/core/typeconverters/qlistqlistqcolortosadvectorsadvectoracolor.cpp \
    ifaceed/core/typeconverters/qstringtosadstring.cpp \
    ifaceed/core/typeconverters/sadacolortoqcolor.cpp \
    ifaceed/core/typeconverters/sadcolortoqcolor.cpp \
    ifaceed/core/typeconverters/sadstringtoqstring.cpp \
    ifaceed/core/typeconverters/sadvectorsadvectoracolortoqlistqlistqcolor.cpp \
    ifaceed/core/editor.cpp \
    ifaceed/core/qttarget.cpp \
    ifaceed/core/saddythread.cpp \
    ifaceed/acolordialog.cpp \
    ifaceed/qwwlongspinbox.cpp \
    ifaceed/qwwulongspinbox.cpp \
    ifaceed/wwglobal_p.cpp \
    ifaceed/history/command.cpp \
    ifaceed/history/history.cpp \
    ifaceed/history/database/newproperty.cpp \
    ifaceed/history/database/removeproperty.cpp \
    ifaceed/gui/anglewidget/anglewidget.cpp \
    ifaceed/gui/colorpicker/colorpicker.cpp \
    ifaceed/gui/colorview/colorview.cpp \
    ifaceed/gui/fontsizewidget/fontsizewidget.cpp \
    ifaceed/gui/rectwidget/rectwidget.cpp \
    ifaceed/gui/resourcetreewidget/cell.cpp \
    ifaceed/gui/resourcetreewidget/celldelegate.cpp \
    ifaceed/gui/resourcetreewidget/defaultimage.cpp \
    ifaceed/gui/resourcetreewidget/resourcecache.cpp \
    ifaceed/gui/resourcetreewidget/resourcetreewidget.cpp \
    ifaceed/gui/table/delegate.cpp \
    ifaceed/gui/table/delegatefactory.cpp \
    ifaceed/gui/table/doubledelegate.cpp \
    ifaceed/gui/table/floatdelegate.cpp \
    ifaceed/gui/table/booldelegate.cpp \
    ifaceed/gui/table/intdelegate.cpp \
    ifaceed/gui/table/longdelegate.cpp \
    ifaceed/gui/table/longlongdelegate.cpp \
    ifaceed/gui/table/sadacolordelegate.cpp \
    ifaceed/gui/table/sadcolordelegate.cpp \
    ifaceed/gui/table/sadstringdelegate.cpp \
    ifaceed/gui/table/shortdelegate.cpp \
    ifaceed/gui/table/signedchardelegate.cpp \
    ifaceed/gui/table/unsignedchardelegate.cpp \
    ifaceed/gui/table/unsignedintdelegate.cpp \
    ifaceed/gui/table/unsignedlongdelegate.cpp \
    ifaceed/gui/table/unsignedlonglongdelegate.cpp \
    ifaceed/gui/table/unsignedshortdelegate.cpp \
    ifaceed/gui/tuplewidget/doubletuplewidget.cpp \
    ifaceed/gui/tuplewidget/int64tuplewidget.cpp \
    ifaceed/gui/table/sadpoint2ddelegate.cpp \
    ifaceed/gui/table/sadsize2ddelegate.cpp \
    ifaceed/gui/table/sadpoint2idelegate.cpp \
    ifaceed/gui/table/sadsize2idelegate.cpp \
    ifaceed/gui/eventfilter.cpp \
    ifaceed/history/scenes/scenelayerswap.cpp \
    ifaceed/history/scenes/scenesadd.cpp \
    ifaceed/history/scenes/sceneschangename.cpp \
    ifaceed/history/scenes/scenesremove.cpp \
    ifaceed/core/synchronization.cpp \
    ifaceed/history/scenenodes/scenenodesnew.cpp \
    ifaceed/gui/labelactions.cpp \
    ifaceed/gui/scenenodeactions.cpp \
    ifaceed/core/borders/activeborder.cpp \
    ifaceed/core/borders/border.cpp \
    ifaceed/core/borders/selectionborder.cpp \
    ifaceed/core/shared.cpp \
    ifaceed/history/label/changefontname.cpp \
    ifaceed/history/label/changefontsize.cpp \
    ifaceed/history/label/changetext.cpp \
    ifaceed/history/label/changelinespacing.cpp \
    ifaceed/history/scenenodes/scenesnodeschangename.cpp \
    ifaceed/core/typeconverters/sadrect2dtoqrectf.cpp \
    ifaceed/core/typeconverters/qrectftosadrect2d.cpp \
    ifaceed/history/scenenodes/scenenodeschangearea.cpp \
    ifaceed/history/scenenodes/scenenodeschangeangle.cpp \
    ifaceed/history/scenenodes/scenenodeschangecolor.cpp \
    ifaceed/history/scenenodes/scenenodeschangepropertywhichlinkedtocheckbox.cpp \
    ifaceed/history/scenenodes/scenenodeschangevisibility.cpp \
    ifaceed/gui/rotationprocess.cpp \
    ifaceed/gui/sprite2dactions.cpp \
    ifaceed/history/sprite2d/changeflipx.cpp \
    ifaceed/history/sprite2d/changeflipy.cpp \
    ifaceed/history/sprite2d/changeoptions.cpp \
    ifaceed/history/sprite2d/makebackground.cpp \
    ifaceed/history/scenenodes/scenenodesremove.cpp \
    ifaceed/history/scenenodes/scenenodeslayerswap.cpp \
    ifaceed/history/scenes/scenesclear.cpp \
    ifaceed/gui/customobjectactions.cpp \
    ifaceed/history/customobject/customobjectchangeschema.cpp \
    ifaceed/core/borders/deletehotspot.cpp \
    ifaceed/core/borders/resizehotspot.cpp \
    ifaceed/core/selection.cpp \
    ifaceed/reloadfilelist.cpp \
    ifaceed/gui/renderways.cpp \
    ifaceed/gui/wayactions.cpp \
    ifaceed/history/ways/waysnew.cpp \
    ifaceed/history/ways/waysremove.cpp \
    ifaceed/history/ways/wayschangename.cpp \
    ifaceed/history/ways/wayschangetotaltime.cpp \
    ifaceed/history/ways/wayschangeclosed.cpp \
    ifaceed/history/ways/wayswaypointnew.cpp \
    ifaceed/history/ways/wayswaypointchange.cpp \
    ifaceed/history/ways/wayswaypointremove.cpp \
    ifaceed/history/ways/wayswaypointswap.cpp \
    ifaceed/gui/dialogueactions.cpp \
    ifaceed/history/dialogues/dialogueschangename.cpp \
    ifaceed/history/dialogues/dialoguesnew.cpp \
    ifaceed/history/dialogues/dialoguesphrasechangeduration.cpp \
    ifaceed/history/dialogues/dialoguesphrasechangelineeditbasedproperty.cpp \
    ifaceed/history/dialogues/dialoguesphrasechangephrase.cpp \
    ifaceed/history/dialogues/dialoguesphrasenew.cpp \
    ifaceed/history/dialogues/dialoguesphraseremove.cpp \
    ifaceed/history/dialogues/dialoguesphraseswap.cpp \
    ifaceed/history/dialogues/dialoguesremove.cpp \
    ifaceed/history/animations/animationsaddtocomposite.cpp \
    ifaceed/history/animations/animationschangeblinkingfrequency.cpp \
    ifaceed/history/animations/animationschangecameraangle.cpp \
    ifaceed/history/animations/animationschangecameraoffset.cpp \
    ifaceed/history/animations/animationschangecamerapivot.cpp \
    ifaceed/history/animations/animationschangecolorcolor.cpp \
    ifaceed/history/animations/animationschangefontlistfonts.cpp \
    ifaceed/history/animations/animationschangefontsizesize.cpp \
    ifaceed/history/animations/animationschangelist.cpp \
    ifaceed/history/animations/animationschangelooped.cpp \
    ifaceed/history/animations/animationschangename.cpp \
    ifaceed/history/animations/animationschangerect.cpp \
    ifaceed/history/animations/animationschangerotateangle.cpp \
    ifaceed/history/animations/animationschangeshakingfrequency.cpp \
    ifaceed/history/animations/animationschangetime.cpp \
    ifaceed/history/animations/animationschangewaymovingway.cpp \
    ifaceed/history/animations/animationsnew.cpp \
    ifaceed/history/animations/animationsremove.cpp \
    ifaceed/history/animations/animationsremovefromcomposite.cpp \
    ifaceed/history/animations/animationsswapincomposite.cpp \
    ifaceed/history/groups/groupsaddinstance.cpp \
    ifaceed/history/groups/groupschangelooped.cpp \
    ifaceed/history/groups/groupschangename.cpp \
    ifaceed/history/groups/groupsnew.cpp \
    ifaceed/history/groups/groupsremove.cpp \
    ifaceed/history/groups/groupsremoveinstance.cpp \
    ifaceed/history/instances/instanceschangename.cpp \
    ifaceed/history/instances/instanceschangenimation.cpp \
    ifaceed/history/instances/instanceschangeobject.cpp \
    ifaceed/history/instances/instanceschangestarttime.cpp \
    ifaceed/history/instances/instanceschangeway.cpp \
    ifaceed/history/instances/instancesnew.cpp \
    ifaceed/history/instances/instancesremove.cpp \
    ifaceed/history/batchcommand.cpp \
    ifaceed/gui/codeedit/codeedit.cpp \
    ifaceed/gui/codeedit/highlighter.cpp \
    ifaceed/gui/codeedit/linenumberarea.cpp \
    ifaceed/gui/textedit/textedit.cpp \
    ifaceed/gui/animationactions.cpp \
    ifaceed/gui/animationgroupprocess.cpp \
    ifaceed/gui/animationinstanceprocess.cpp \
    ifaceed/gui/animationprocess.cpp \
    ifaceed/gui/groupactions.cpp \
    ifaceed/gui/instanceactions.cpp \
    ifaceed/scripting/scripting.cpp \
    ifaceed/scripting/tovalue.cpp \
    ifaceed/scripting/callable.cpp \
    ifaceed/scripting/fromvalue.cpp \
    ifaceed/scripting/multimethod.cpp \
    ifaceed/scripting/queryobject.cpp \
    ifaceed/scripting/scriptinglog.cpp \
    ifaceed/scripting/classwrapper.cpp \
    ifaceed/scripting/point2d.cpp \
    ifaceed/scripting/point2i.cpp \
    ifaceed/scripting/point3d.cpp \
    ifaceed/scripting/point3i.cpp \
    ifaceed/scripting/rect2d.cpp \
    ifaceed/scripting/rect2i.cpp \
    ifaceed/scripting/acolor.cpp \
    ifaceed/scripting/color.cpp \
    ifaceed/scripting/size2d.cpp \
    ifaceed/scripting/size2i.cpp \
    ifaceed/scripting/scenes/scenesnamesetter.cpp \
    ifaceed/scripting/scenes/scenesbindings.cpp \
    ifaceed/scripting/database/databasebindings.cpp \
    ifaceed/scripting/querytable.cpp \
    ifaceed/scripting/scenenodes/scenenodesbindings.cpp \
    ifaceed/scripting/queryresource.cpp \
    ifaceed/scripting/scenenodes/scenenodesareasetter.cpp \
    ifaceed/scripting/scenenodes/scenenodesfontsizesetter.cpp \
    ifaceed/scripting/scenenodes/scenenodesfontsetter.cpp \
    ifaceed/scripting/scenenodes/scenenodesflagsetter.cpp \
    ifaceed/scripting/scenenodes/scenenodesoptionssetter.cpp \
    ifaceed/scripting/scenenodes/scenenodesschemasetter.cpp \
    ifaceed/scripting/ways/waysbindings.cpp \
    ifaceed/scripting/animations/animationsbindings.cpp \
    ifaceed/scripting/dialogues/dialoguesbindings.cpp \
    ifaceed/scripting/dialogues/dialoguesphraseref.cpp \
    ifaceed/scripting/ways/wayspointref.cpp \
    ifaceed/scripting/animations/animationswaysetter.cpp \
    ifaceed/gui/codeedit/completer.cpp \
    ifaceed/scripting/isaabb.cpp \
    ifaceed/scripting/instances/instancesbindings.cpp \
    ifaceed/scriptinghelp.cpp \
    ifaceed/scripting/groups/groupsbindings.cpp \
    ifaceed/scripting/groups/groupsloopedsetter.cpp \
    ifaceed/scripting/groups/groupsnamesetter.cpp \
    ifaceed/scripting/instances/instancesanimationdbsetter.cpp \
    ifaceed/scripting/instances/instancesanimationsetter.cpp \
    ifaceed/scripting/instances/instancesnamesetter.cpp \
    ifaceed/scripting/instances/instancesobjectsetter.cpp \
    ifaceed/scripting/instances/instancesstarttimesetter.cpp \
    ifaceed/scripting/instances/instanceswaysetter.cpp \
    ifaceed/history/animations/animationschangepropertyaspoint2displayedintwospinboxes.cpp \
    ifaceed/history/animations/animationschangeresizeendingsize.cpp \
    ifaceed/history/animations/animationschangeresizestartingsize.cpp


DESTDIR = ../../bin/ifaceed/

unix {
	DEFINES += "UNIX=1"
	DEFINES += "LINUX=1"
	DEFINES += "GCC=1"
}

win32 {
	DEFINES += "WIN32=1"
	DEFINES +=  "MINGW=1"
}

CONFIG(debug, debug|release) {
    LIBS += -L../../lib/ -lsaddy-debug -lsaddy-ft-debug
    TARGET = iface-editor-debug
}

CONFIG(release, debug|release) {
    LIBS += -L../../lib/ -lsaddy-release -lsaddy-ft-release
    TARGET = iface-editor-release
}

unix {
        LIBS += -lGL
        LIBS += -lGLU
        LIBS += -lX11
        LIBS += -lfreetype
}

win32 {
        QMAKE_LIBDIR += $(FREETYPE_LIB)
        LIBS += -lglu32 -lopengl32 -lkernel32 -luser32 -lgdi32 -lwinspool  -lshell32 -lcomdlg32 -ladvapi32 -lfreetype
}

QMAKE_CXXFLAGS += -Wno-reorder -Wno-unused -Wno-sign-compare -w
