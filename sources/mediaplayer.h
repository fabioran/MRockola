/*
 * MRockola.
 * Copyright (C) 2011-2016  Edgar Armando Lopez Hernandez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more open details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: inge_lopez@yahoo.com
 */

#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QQuickPaintedItem>
#if defined(QT_OPENGL_ES_2)
    #include <QtGui/QOpenGLFunctions_ES2>
#else
    #include <QtGui/QOpenGLFunctions_3_0>
#endif
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>



#include <vlc/vlc.h>
#include "database.h"

#define VOL_DEFAULT 5


class MediaPlayer : public QQuickPaintedItem
{
    Q_OBJECT

    struct avStack {
        int type;
        QString path;
    };

    enum PaintMode
    {
        PaintModeQPainter,
        PaintModeTexture,
        PaintModeShaders
    };


public:
    explicit MediaPlayer(QQuickItem *parent = 0);
    ~MediaPlayer();

    bool getEqualizerEnable() { return equalizerEnable; };
    void setEqualizerBands(QString values);
    void setEqualizerEnable(bool set) { equalizerEnable = set; };

    Q_INVOKABLE void playNext();
    Q_INVOKABLE void setQueue(QString song, int type);
    Q_INVOKABLE void setVolumeUp() { setVolume(VOL_DEFAULT); };
    Q_INVOKABLE void setVolumeDown() { setVolume(-VOL_DEFAULT); };
    Q_INVOKABLE qint64 getCurrentTime();
    qint64 setTape();

    void paint(QPainter *painter);

signals:
    void finishTrack();
    void isVideo(bool b);
    void progressBarSetMax(int max);
    void progressBarSetValue(int value);
    
public slots:
    void updateInterface();

protected slots:
    unsigned int setupFormat(char *chroma, unsigned int *width, unsigned int *height,
                               unsigned int *pitches, unsigned int *lines);
    void updateTexture();//void *picture, void * const *planes);
    void paintFrame();

private:
    void play();
    void skip();
    void setCurrentSource(avStack data);
    void setEqualizerBands();
    void setVolume(const int &vol);

    static void *vlcVideoLockCallBack(void *object, void **planes);
    static void vlcVideoUnlockCallback(void *object, void *picture, void * const *planes);

    static void vlcVideoDisplayCallback(void *object, void *picture);
    static unsigned int vlcVideoFormatCallback(void **object, char *chroma, unsigned int *width, unsigned int *height,
                                  unsigned int *pitches, unsigned int *lines);

    void updateBoundingRect();

    void setupPlanes(char *chroma, unsigned int *width, unsigned int *height,
                     unsigned int *pitches, unsigned int *lines);
    void setupBuffers();
    void cleanupBuffers();
    void setupTextures();
    void cleanupTextures();
    void updateTextures();


private:
    QOpenGLShaderProgram *m_program;

    bool            equalizerEnable;
    QString         equalizerValues;
    bool            isEmpty;
    bool            setEqualizer;
    int             volume;
    int             nCount;

    QList<avStack>  m_stack;

    //audio_output_t *getAudioOut( libvlc_media_player_t *mp );
    //input_thread_t *getInputThread( libvlc_media_player_t *p_mi );
    libvlc_instance_t*      m_libVlc;
    libvlc_media_player_t*  m_vlcPlayer;
    libvlc_time_t AllQueueTime; // Soh pra ver se fica...

    //Video Properties
    QString m_fileName;
    quint32 m_width;
    quint32 m_height;

    //State and buffer variables
    uchar *m_pixelBuff;
    PaintMode m_paintMode;
    quint32 m_textureId;
    int m_numPlanes;
};

#endif // MEDIAPLAYER_H
