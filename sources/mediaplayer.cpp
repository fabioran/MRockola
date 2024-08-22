/*
 * MRockola.
 * Copyright (C) 2011-2017  Edgar Armando Lopez Hernandez
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

#include <QDir>
#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <QtDebug>

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#endif

#include "mediaplayer.h"
#include "constants.h"
#include <vlc/libvlc_media_player.h>


#define SLIDER_MAX      1000
const QString equalizerReset = "0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00";

//------------------------------------------------------------------------------

MediaPlayer::MediaPlayer(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_program(nullptr),
#ifdef Q_OS_LINUX
    m_paintMode(PaintModeShaders)
#else
    m_paintMode(PaintModeShaders)
#endif
{
    m_pixelBuff = nullptr;
    isEmpty = true;

    setRenderTarget(InvertedYFramebufferObject);
    setFlag(ItemHasContents, true);

    setSmooth(true);

    m_textureId = 0;

    m_vlcPlayer = nullptr;
    setEqualizer = false;
    equalizerValues = equalizerReset;
    equalizerEnable = false;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateInterface()));
    timer->start(100);


    char const *vlc_argv[] =
    {
        "--intf=dummy",
        "--ignore-config",
        "--no-media-library",
        "--no-osd",
        "--no-snapshot-preview",
        "--no-stats",
        "--no-video-title-show",
        "-vvv"
    };
    // Init libVLC
    m_libVlc =  libvlc_new(sizeof(vlc_argv) / sizeof(vlc_argv[0]), vlc_argv);

}

//------------------------------------------------------------------------------

MediaPlayer::~MediaPlayer()
{
    skip();
    m_stack.clear();

    if(m_vlcPlayer)
    {
        libvlc_media_player_release(m_vlcPlayer);
    }

    if(m_libVlc)
    {
        libvlc_release(m_libVlc);
    }

    cleanupBuffers();
    cleanupTextures();

    if(m_program)
    {
        delete m_program;
        m_program = nullptr;
    }
}

//------------------------------------------------------------------------------

void MediaPlayer::setQueue(QString song, int type)
{
    // qint64 total
    // QMessageBox Msgbox;
    nCount += 1;
    double msec = AllQueueTime, seg = msec / 1000.0, min = seg / 60.0;
    // Msgbox.setText("PARADA do song: "+QString::number(min) + song);
    // Msgbox.exec();
    qDebug("minutes here: %d", min);
    // double min = 10;
    if (min < 10) {
        avStack data;
        data.path = song;
        data.type = type;
        m_stack.append(data);
        // Msgbox.setText("setQueue >>> min: "+QString::number(min));
        // Msgbox.exec();
        qDebug("song here: ");
        qDebug() << song;

        if(isEmpty)
        {
            isEmpty = false;
            playNext();
            return;
        }
    }
    else {
        // Msgbox.setText("SetQueuePorra"+QString::number(nCount));
        // Msgbox.setText("SetQueuePorra");
        // Msgbox.exec();
        qDebug("nCount: ");
        qDebug() << nCount;
    }
    // Msgbox.setText("setQueue -> nCount: "+QString::number(nCount));
    // Msgbox.exec();
    qDebug("end setQueue -->>>");


}

/***
* GetTimeStr
*
*/
// qint64 QString MediaPlayer::getCurrentTime() // Copiado do update
qint64 MediaPlayer::getCurrentTime() // Copiado do update
{
    QString strTotal;

    // AllQueueTime += libvlc_media_player_get_length(m_vlcPlayer);
    // libvlc_time_t\ elapsedTime;

    if(m_vlcPlayer)  {
        // libvlc_time_t elapsedTime = libvlc_media_player_get_time(m_vlcPlayer);
        // elapsedTime = libvlc_media_get_duration(vMedia);


        int state = libvlc_media_player_get_state(m_vlcPlayer);
        if (state == libvlc_Buffering || state == libvlc_Playing) {
          AllQueueTime = libvlc_media_player_get_length(m_vlcPlayer);
          AllQueueTime = AllQueueTime+0;
          qDebug("AllQueueTime -> pass");
          qDebug() << AllQueueTime;
        }

    } // Vai

    double msec = AllQueueTime, seg = msec / 1000.0, min = seg / 60.0;
    seg = qint64((min - qint64(min)) * 60.0);
    min = qint64(min);
    strTotal = QString::number(min) + ":" + QString::number(seg);
    /*
    auto nMin = min;
    msec = elapsedTime*AllQueueTime;
    seg = msec / 1000.0;
    min = seg / 60.0;
    seg = qint64((min - qint64(min)) * 60.0);
    min = qint64(min);
    auto strCurrent = QString::number(min) + ":" + QString::number(seg);
    */
    // return (strCurrent + "/" + strTotal);
    return qint64(min);

    // return "VAI";

}

/***
* setTape (usar para fila pegando query na tabela playlist)
* 04/05/2022
*
*/
qint64 MediaPlayer::setTape() {
    QString strTotal;

    //QList<get2PlayTable> sPaths;
    // QVariantList sPath;
    // sPath = DataBase::get2PlayTable(50);

    if(m_vlcPlayer)  {
        int state = libvlc_media_player_get_state(m_vlcPlayer);
        if (state == libvlc_Buffering || state == libvlc_Playing)
          AllQueueTime = AllQueueTime+0;
    } // Vai

    double msec = AllQueueTime, seg = msec / 1000.0, min = seg / 60.0;
    seg = qint64((min - qint64(min)) * 60.0);
    min = qint64(min);
    strTotal = QString::number(min) + ":" + QString::number(seg);

    return qint64(min);

}

//------------------------------------------------------------------------------

void MediaPlayer::skip()
{
    if(m_vlcPlayer)
    {
        libvlc_media_player_stop(m_vlcPlayer);
        libvlc_media_player_release(m_vlcPlayer);
    }
    m_vlcPlayer = nullptr;
    emit progressBarSetValue(0);
}

//------------------------------------------------------------------------------

void MediaPlayer::setVolume(const int &vol)
{
    if(m_vlcPlayer)
    {
        volume = libvlc_audio_get_volume(m_vlcPlayer);
        libvlc_audio_set_volume(m_vlcPlayer, volume + vol);
    }
}

//------------------------------------------------------------------------------

void MediaPlayer::playNext()
{
    emit isVideo(false);
    emit finishTrack();
    qDebug("mediaPlayNext -> finishTrack");

    if(!m_stack.isEmpty())
    {
        avStack data = m_stack.takeFirst();

        if(data.type == MRockola::MediaVideo || data.type == MRockola::MediaKaraokeCDG )
        {
            emit isVideo(true);
        }

        setCurrentSource(data);
    }
    else
    {
        isEmpty = true;
        skip();
    }
    qDebug("mediaPlayNext -> end");
}

//------------------------------------------------------------------------------

void MediaPlayer::updateInterface() //Update interface and check if song is finished
{    
    if(m_vlcPlayer)
    {
        // AllQueueTime =+ libvlc_media_player_get_length(m_vlcPlayer);
        int state = libvlc_media_player_get_state(m_vlcPlayer);
        if(state == libvlc_Buffering || state == libvlc_Playing)
        {
            setEqualizerBands();
            libvlc_time_t fullTime = libvlc_media_player_get_length(m_vlcPlayer);
            emit progressBarSetMax(fullTime);
            libvlc_time_t elapsedTime = libvlc_media_player_get_time(m_vlcPlayer);
            emit progressBarSetValue(elapsedTime);
        }
        else if(state == libvlc_Ended  || state == libvlc_Error ||
                 state == libvlc_Stopped )
        {
            emit progressBarSetValue(0);
            playNext();
        }
    }
}

//------------------------------------------------------------------------------

void MediaPlayer::setCurrentSource(avStack data)
{
    QMessageBox Msgbox;
    QString strTime;
    // libvlc_media_track_t ***nTracks = nullptr;
    // uint AllTracks;

    if(m_vlcPlayer &&
        libvlc_media_player_is_playing(m_vlcPlayer)
       )
        skip();

#ifdef Q_OS_WIN
    data.path = QDir::toNativeSeparators(data.path);
#endif

    // New Media
    libvlc_media_t *vlcMedia = libvlc_media_new_path(m_libVlc, data.path.toUtf8().constData());
    // Msgbox.setText("NEW MEDIA -> setCurrentSource");
    // Msgbox.exec();
    qDebug("NEW MEDIA -> setCurrentSource");
    qDebug() << data.path.toUtf8().constData();

    if( !vlcMedia  )
    {
        playNext();
        return;
    }

#ifdef Q_OS_WIN
    if(data.type == MRockola::MediaKaraokeCDG)
    {
        data.path.chop(4);  //remove extension file .mp3
        QString str =  QString("sub-file=%1%2").arg(data.path).arg(".cdg");
       libvlc_media_add_option(vlcMedia, str.toUtf8().constData());
    }
#endif

    m_vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);
    libvlc_media_release(vlcMedia);

    // Integrate the video in the interface
    if(data.type == MRockola::MediaVideo || data.type == MRockola::MediaKaraokeCDG )
    {
        libvlc_video_set_callbacks(m_vlcPlayer, vlcVideoLockCallBack, vlcVideoUnlockCallback, vlcVideoDisplayCallback,
                                   this);
        libvlc_video_set_format_callbacks(m_vlcPlayer, vlcVideoFormatCallback, nullptr);
    }
    // And play
    libvlc_media_player_play(m_vlcPlayer);
    setEqualizer = false;
    // AllTracks = libvlc_media_tracks_get(vlcMedia, nTracks);
    if (libvlc_media_get_parsed_status(vlcMedia)) {
      AllQueueTime = libvlc_media_get_duration(vlcMedia);
      // AllQueueTime += 1;
      double msec = AllQueueTime, seg = msec / 1000.0, min = seg / 60.0;
      strTime = QString::number(min, 'g', 4);
      Msgbox.setText("AFTER Parser: "+strTime);
      Msgbox.exec();
      // libvlc_media_player_stop(m_vlcPlayer);
      // libvlc_media_player_play(m_vlcPlayer);
    }
    qDebug("setCurrentSource -> end process");



}

//------------------------------------------------------------------------------

void MediaPlayer::setEqualizerBands()
{
    //if(!setEqualizer && equalizerEnable)
        //setEqualizerBands(equalizerValues);
    setEqualizer = true;
}


//------------------------------------------------------------------------------

void MediaPlayer::paint(QPainter *painter)
{
    switch(m_paintMode)
    {
    case PaintModeQPainter:
    {
        QImage img((uchar *)m_pixelBuff, m_width, m_height, QImage::Format_RGB888);
        painter->drawImage(boundingRect(), img.rgbSwapped(), QRect(0,0,m_width, m_height));
    }
        break;

    case PaintModeTexture:
    {
        painter->beginNativePainting();

#if defined(QT_OPENGL_ES_2)
        if(m_textureId       {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }
#else
        QRectF rect = boundingRect();

        if(m_textureId == 0)
        {
            setupTextures();
        }
        else
        {

         updateTextures();
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, m_textureId);

        glBegin(GL_QUADS);
            glTexCoord2d(0.0,0.0);
            glVertex2d(rect.x(), rect.y());
            glTexCoord2d(1.0,0.0);
            glVertex2d(rect.x(), rect.y() + rect.height());
            glTexCoord2d(1.0,1.0);
            glVertex2d(rect.x() + rect.width(), rect.y() + rect.height());
            glTexCoord2d(0.0,1.0);
            glVertex2d(rect.x() + rect.width(), rect.y());
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        }
#endif
        painter->endNativePainting();
    }
        break;
    case PaintModeShaders:
    {
        QRectF rect = boundingRect();

        painter->beginNativePainting();

        if (!m_program)
        {
            m_program = new QOpenGLShaderProgram();
            m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                               "attribute highp vec4 vertices;"
                                               "attribute highp vec2 texUV;"
                                               "varying highp vec2 texcoord;"
                                               "void main() {"
                                               "    gl_Position = vertices;"
                                               "    texcoord = texUV;"
                                               "}");
            m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                               "uniform sampler2D tex;"
                                               "varying highp vec2 texcoord;"
                                               "void main() {"
                                               "    gl_FragColor = texture2D(tex, texcoord);"
                                               "}");

            m_program->bindAttributeLocation("vertices", 0);
            m_program->bindAttributeLocation("texUV", 1);

            m_program->link();

            setupTextures();
       }
        else
        {
        const GLfloat targetVertex[] =
        {
           -1,-1,
            1,-1,
           -1, 1,
            1, 1
        };

        const float textureCoors[] = {
            0, 1, // bottom left
            1, 1, // bottom right
            0, 0, // top left
            1, 0, // top right
        };


        m_program->bind();

        m_program->enableAttributeArray(0);
        m_program->enableAttributeArray(1);

        m_program->setAttributeArray(0, GL_FLOAT, targetVertex, 2);
        m_program->setAttributeArray(1, textureCoors, 2);

        glViewport(0, 0, rect.width(), rect.height());

        glDisable(GL_DEPTH_TEST);

        glClearColor(0, 0, 0, 1);

        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

#if defined(QT_OPENGL_ES_2)
        glActiveTexture(GL_TEXTURE0);
#endif
        updateTextures();

        m_program->setUniformValue("tex", 0);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

        m_program->disableAttributeArray(1);
        m_program->disableAttributeArray(0);
        m_program->release();
        }

        painter->endNativePainting();
    }
        break;
    }
}

//------------------------------------------------------------------------------

unsigned int MediaPlayer::setupFormat(char *chroma, unsigned int *width, unsigned int *height, unsigned int *pitches, unsigned int *lines)
{
    setupPlanes(chroma, width, height, pitches, lines);
    setupBuffers();

    return(m_numPlanes);
}

//------------------------------------------------------------------------------

unsigned int MediaPlayer::vlcVideoFormatCallback(void **object, char *chroma, unsigned int *width, unsigned int *height,
                           unsigned int *pitches, unsigned int *lines)
{
    unsigned int retval = 0;
    MediaPlayer *instance = (MediaPlayer *)*object;
    QMetaObject::invokeMethod(instance, "setupFormat", Qt::BlockingQueuedConnection, Q_RETURN_ARG(unsigned int, retval),
                              Q_ARG(char *, chroma), Q_ARG(unsigned int *, width), Q_ARG(unsigned int *, height),
                              Q_ARG(unsigned int *, pitches), Q_ARG(unsigned int *, lines));

    return(retval);
}

//------------------------------------------------------------------------------

void *MediaPlayer::vlcVideoLockCallBack(void *object, void **planes)
{
    //Lock the pixel mutex, and hand the pixel buffer to VLC
    MediaPlayer *instance = (MediaPlayer *)object;
    //QMutexLocker(&(instance->m_pixelMutex));  //vlc 2.1.5 crash ???
    planes[0] = (void *)instance->m_pixelBuff;
    return NULL;
}

//------------------------------------------------------------------------------

void MediaPlayer::vlcVideoUnlockCallback(void *object, void *picture, void * const *planes)
{
    Q_UNUSED(picture);
    Q_UNUSED(planes);
    Q_UNUSED(object)

    //MediaPlayer *instance = (MediaPlayer *)object;
    //QMetaObject::invokeMethod(instance, "updateTexture",  Qt::BlockingQueuedConnection);
}

//------------------------------------------------------------------------------

void MediaPlayer::vlcVideoDisplayCallback(void *object, void *picture)
{
    Q_UNUSED(picture);
    //Call the paintFrame function in the main thread.
    MediaPlayer *instance = (MediaPlayer *)object;
    QMetaObject::invokeMethod(instance, "paintFrame", Qt::AutoConnection);
}

//------------------------------------------------------------------------------

void MediaPlayer::updateTexture()//void *picture, void * const *planes)
{
    updateTextures();
}

//------------------------------------------------------------------------------

void MediaPlayer::paintFrame()
{
    //Just signal that we need to repaint the item.
    update();
}

//------------------------------------------------------------------------------

void MediaPlayer::setupPlanes(char *chroma, unsigned int *width, unsigned int *height,
                 unsigned int *pitches, unsigned int *lines)
{
    strcpy(chroma, "RV24");
    pitches[0] = *width * 3;
    lines[0] = *height * 3;
    m_width = *width;
    m_height = *height;
    m_numPlanes = 1;
}

//------------------------------------------------------------------------------

void MediaPlayer::setupBuffers()
{
    m_pixelBuff = new uchar[m_width*m_height*3];
}

//------------------------------------------------------------------------------

void MediaPlayer::cleanupBuffers()
{
    delete m_pixelBuff;
}

//------------------------------------------------------------------------------

void MediaPlayer::setupTextures()
{
    switch(m_paintMode) {
    case PaintModeQPainter:
        break;
    case PaintModeTexture:
    case PaintModeShaders:
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//#if defined(QT_OPENGL_ES_2)
//        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//#endif
        glBindTexture(GL_TEXTURE_2D, 0);
        break;
    }
}

//------------------------------------------------------------------------------

void MediaPlayer::cleanupTextures()
{
    switch(m_paintMode) {
    case PaintModeQPainter:
        break;
    case PaintModeTexture:
    case PaintModeShaders:
        if(m_textureId != 0){glDeleteTextures(1,&m_textureId);}
        break;
    }
}

//------------------------------------------------------------------------------

void MediaPlayer::updateTextures()
{
    switch(m_paintMode) {
    case PaintModeQPainter:
        break;
    case PaintModeTexture:
    case PaintModeShaders:
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
#if defined(QT_OPENGL_ES_2)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixelBuff);
#else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, m_pixelBuff);
#endif
        //glBindTexture(GL_TEXTURE_2D, 0);
        break;
    }
}

//------------------------------------------------------------------------------
/*
input_thread_t * MediaPlayer::getInputThread( libvlc_media_player_t *p_mi )
{
    input_thread_t *p_input_thread;

    vlc_mutex_lock(&p_mi->input.lock);
    p_input_thread = p_mi->input.p_thread;

    if( p_input_thread )
        vlc_object_hold( p_input_thread );
    else
        libvlc_printerr( "No active input" );

    vlc_mutex_unlock(&p_mi->input.lock);
    return p_input_thread;
}

//------------------------------------------------------------------------------

audio_output_t *MediaPlayer::getAudioOut( libvlc_media_player_t *mp )
{
    input_thread_t *p_input = getInputThread( mp );
    if( p_input == NULL )
        return NULL;

    audio_output_t * p_aout = input_GetAout(p_input);
    vlc_object_release( p_input );

    if( p_aout == NULL )
        libvlc_printerr( "No active audio output" );

    return p_aout;
}

//------------------------------------------------------------------------------

void MediaPlayer::setEqualizerBands(QString values)
{
    equalizerValues = values;

    if(vlcPlayer == NULL)
        return;

   vlc_object_t *p_aout = (vlc_object_t *)getAudioOut(vlcPlayer);

   if( p_aout )
   {
        int size = values.toStdString().length();
        char psz_values[size];
        strcpy(psz_values, values.toStdString().c_str());

        aout_EnableFilter(p_aout, "equalizer", true );
        var_SetString( p_aout, "equalizer-bands", psz_values );
        //const char *psz_bands = var_GetNonEmptyString( p_aout, "equalizer-bands" );
   }
}

*/
