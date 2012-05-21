#ifndef LOAD_THREAD_H
#define LOAD_THREAD_H
// ****************************************************************************
//  load_thread.h                                                  Tao project
// ****************************************************************************
//
//   File Description:
//
//    Load a 3D model file asynchronously using GLC_Lib
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************


#include <QFile>
#include <QThread>
#include <GLC_Factory>
#include <GLC_World>


class LoadThread : public QThread
// ----------------------------------------------------------------------------
//   Load a 3D object file in a new thread
// ----------------------------------------------------------------------------
{
public:
    LoadThread(QString path) : path(path) {}
    ~LoadThread() {}

    // Load the file
    virtual void   run();

signals:
    void           percentComplete(int percent);
    void           failed();

public:
    QString        path;
    GLC_World      world;
    QString        error;

    Q_OBJECT
};

#endif // LOAD_THREAD_H
