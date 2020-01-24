#include "joystick.h"

// Необходимы хэдеры для работы с джойстиком на линуксе
#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <glob.h>
#include <string.h>
// Структуры, константы и т.п. для работы с джойстиком
// см. kernel.org/Documentation/input/joystick-api.txt либо аналогичный путь в исходниках ядра
// Разработка велась с использованием ядра 3.13, в более новых версиях ядра интерфейс может поменяться.
#include <linux/joystick.h>

int fd = 0; // joystick device file descriptor

#endif

#ifdef WIN32
#include <Windows.h>
#endif

Joystick::Joystick() {
    rusWork = rudWork = false;
    memset((void*)&_params,0,sizeof(_params));
}

Joystick::~Joystick() {
#ifdef __linux__
    if (fd > 0) close(fd);
#endif
}

bool Joystick::rusStatus() {
    checkStatus();
    return rusWork;
}

bool Joystick::rudStatus() {
    checkStatus();
    return rudWork;
}

void Joystick::checkStatus() {
#ifdef __linux__
    if (!rusWork) {
        glob_t m;
        int code;
        // Ищем все джойстики и выбираем среди них Thrustmaster
        if ( (code = glob("/dev/input/js*", 0, NULL, &m)) == 0) {
            for (size_t i = 0; i < m.gl_pathc; i++) {
                if (fd > 0) close(fd);
                fd = open(m.gl_pathv[i], O_RDONLY | O_NONBLOCK);
                if (fd > 0) {
                    char *id_string = new char[JOYNAMELEN];
                    ioctl(fd, JSIOCGNAME(JOYNAMELEN), id_string);
                    // Почему-то драйвер иногда записывает на один символ больше
                    if (strlen(id_string) > JOYNAMELEN) {
                        id_string[JOYNAMELEN] = '\0';
                    }
                    // Джойстик подключен, но не Thrustmaster
                    if (strcmp(id_string, JOYNAME)) {
                        close(fd);
                    } else {
                        rusWork = true;
                    }
                    delete id_string;
                    if (rusWork) break;
                }
            }
        } else {
            if (code == GLOB_NOMATCH) {
                // no joystick found, just empty for now
            }
        }
        globfree(&m);
    } else if (!rudWork) {
        char naxes, nbuttons;
        ioctl(fd, JSIOCGAXES, &naxes);
        ioctl(fd, JSIOCGBUTTONS, &nbuttons);
        if (naxes == NAXES && nbuttons == NBTNS) {
            rudWork = true;
        }
    }
#endif
}

void Joystick::fillRUSParams(TRUSParams &p) {
    fillParams();
    p = _params.rusParams;
}

void Joystick::fillRUDParams(TRUDParams &p) {
    fillParams();
    p = _params.rudParams;
//    memcpy(p, (void *)&_params.rudParams, sizeof(TRUDParams));
//    p = _params.rud_params;
}

void Joystick::fillParams() {
#ifdef __linux__
    l_fillParams();
#endif
#ifdef WIN32
    w_fillParams();
    k_fillParams();
#endif
}

#ifdef __linux__
void Joystick::l_fillParams() {
    if (!rusWork) return;
    // считываем данные и обрабатываем их, пока они есть (файл открыт в неблокирующем режиме)
    errno = 0;
    struct js_event e;
    while (read(fd, &e, sizeof(struct js_event)) > 0) {
        /*
         * Маппинг изменений параметров джойстика на структуру с параметрами
         * Протестировать нажатие кнопок и движение осей на линуксе
         * можно при помощи утилиты jstest из пакета joystick
         */
        switch(e.type & ~JS_EVENT_INIT) {
        case JS_EVENT_BUTTON:
            switch (e.number) {
            /* РУС */
            case 0: /* trigger */ _params.rusParams.trigger = e.value; break;
            case 1: /* wpn_rel */ _params.rusParams.wpn_rel = e.value; break;
            case 2: /* bottom */ _params.rusParams.bottom = e.value; break;
            case 3: /* bottom_trigger */ _params.rusParams.bottom_trigger = e.value; break;
            case 4: /* nws */ _params.rusParams.nws= e.value; break;
            case 5: /* trigger strong */ _params.rusParams.trigger_strong = e.value; break;
            /* 4-pos left */
            case 6: /* 4pos left up */ _params.rusParams.bottomleft.vpos = e.value ? UP : CENTER; break;
            case 7: /* 4pos left right */ _params.rusParams.bottomleft.hpos = e.value ? DOWN : CENTER; break;
            case 8: /* 4pos left down */ _params.rusParams.bottomleft.vpos = e.value ? DOWN : CENTER; break;
            case 9: /* 4pos left left */ _params.rusParams.bottomleft.hpos  = e.value ? UP: CENTER; break;
            /* 4-pos right */
            case 10: /* 4pos right up */ _params.rusParams.bottomright.vpos = e.value ? UP : CENTER; break;
            case 11: /* 4pos right right */ _params.rusParams.bottomright.hpos = e.value ? DOWN : CENTER; break;
            case 12: /* 4pos right down */ _params.rusParams.bottomright.vpos = e.value ? DOWN : CENTER; break;
            case 13: /* 4pos right left */ _params.rusParams.bottomright.hpos = e.value ? UP : CENTER; break;
            /* 4-pos thumb */
            case 14: /* 4pos thumb up */ _params.rusParams.thumb.vpos = e.value ? UP : CENTER; break;
            case 15: /* 4pos thumb right */ _params.rusParams.thumb.hpos = e.value ? DOWN : CENTER; break;
            case 16: /* 4pos thumb down */ _params.rusParams.thumb.vpos = e.value ? DOWN : CENTER; break;
            case 17: /* 4pos thumb left */ _params.rusParams.thumb.hpos = e.value ? UP : CENTER; break;
            /* РУД */
            case 18: /* enable */ _params.rudParams.enable = e.value; break;
            /* 4-pos (v|u)hf + iff in/out */
            case 19: /* vhf */ _params.rudParams.vuhf_iff.vpos= e.value ? UP : CENTER; break;
            case 20: /* uhf */ _params.rudParams.vuhf_iff.vpos= e.value ? DOWN : CENTER; break;
            case 21: /* iff in */ _params.rudParams.vuhf_iff.hpos= e.value ? DOWN : CENTER; break;
            case 22: /* iff out */ _params.rudParams.vuhf_iff.hpos= e.value ? UP : CENTER; break;
            case 23: /* uncage */ _params.rudParams.uncage = e.value; break;
            /* 3-pos dog fight */
            case 24: /* down*/ _params.rudParams.dog_fight.pos = e.value ? DOWN : CENTER; break;
            case 25: /* up */ _params.rudParams.dog_fight.pos = e.value ? UP : CENTER; break;
            /* 3-pos spd brk */
            case 26: /* down */ _params.rudParams.spd_brk.pos = e.value ? DOWN : CENTER; break;
            case 27: /* up */ _params.rudParams.spd_brk.pos = e.value ? UP : CENTER; break;
            default:
                break;
            }
            break;
        case JS_EVENT_AXIS:
            // 0 и 1 оси всегда оси РУС
            switch (e.number) {
            case 0: /* Х-ось РУС */
                _params.rusParams.x = e.value / 32767.0;
                break;
            case 1: /* Y-ось РУС */
                _params.rusParams.y = e.value / 32767.0;
                break;
            case 2: /* Ось РУД или trim, если РУД не подключен */
                if (!rudWork) {
                    if (e.value > 0) _params.rusParams.trim.hpos = DOWN;
                    else if (e.value < 0) _params.rusParams.trim.hpos = UP;
                    else _params.rusParams.trim.hpos = CENTER;
                } else {
                    _params.rudParams.z = e.value / 32767.0;
                }
                break;
            case 3: /* Ant elev или trim, если РУД подключен*/
                if (!rudWork) {
                    if (e.value > 0) _params.rusParams.trim.vpos = DOWN;
                    else if (e.value < 0) _params.rusParams.trim.vpos = UP;
                    else _params.rusParams.trim.vpos = CENTER;
                } else {
                    _params.rudParams.ant_elev = e.value / 32767.0;
                }
                break;
            case 4: /* man rng */
                _params.rudParams.man_rng = e.value / 32767.0;
                break;
            case 5: /* trim, если подключен РУД */
                if (e.value > 0) _params.rusParams.trim.hpos = DOWN;
                else if (e.value < 0) _params.rusParams.trim.hpos  = UP;
                else _params.rusParams.trim.hpos  = CENTER;
                break;
            case 6: /* trim, если подключен РУД */
                if (e.value > 0) _params.rusParams.trim.vpos  = DOWN;
                else if (e.value < 0) _params.rusParams.trim.vpos  = UP;
                else _params.rusParams.trim.vpos  = CENTER;
                break;
            default:
                break;
            }
        }
    }
    /*
     * Все существующие события обработаны.
     * Если события закончились, код ошибки будет EAGAIN (см. иерархию errno.h)
     * В противном случае либо джойстик отключили, либо что-то еще пошло не так.
     */
    // EAGAIN - конец данных в неблокирующем режиме
    if (errno != EAGAIN) {
        if (errno == ENODEV) { // no such device error - joystick unplugged or RUD plugged
            if (fd) close(fd);
            rusWork = rudWork = false;
            checkStatus();
        } else {
            qCritical("Error number: %d", errno);
        }
    }
}
#endif

#ifdef WIN32
void Joystick::w_fillParams() {
    JOYINFO joyinfo;
    if(joyGetPos(JOYSTICKID2, &joyinfo) == JOYERR_NOERROR) {
        _params.rusParams.x = -(2 * joyinfo.wXpos / 65535.0 - 1.0);
        _params.rusParams.y = (2 * joyinfo.wYpos / 65535.0 - 1.0);
        _params.rudParams.z = (1.0 - 1.0 * joyinfo.wZpos / 65535.0);
        rusWork = true;
    } else {
        rusWork = false;
    }

    JOYINFO joyinfo2;
    if(joyGetPos(JOYSTICKID1, &joyinfo2) == JOYERR_NOERROR) {
        _params.rudParams.x = -(2 * joyinfo2.wXpos / 65535.0 - 1.0);
        _params.rudParams.y = (2 * joyinfo2.wYpos / 65535.0 - 1.0);
        _params.rudParams.z = (1.0 - 1.0 * joyinfo2.wZpos / 65535.0);
        rudWork = true;
    } else {
        rudWork = false;
    }
}
#endif
#ifdef WIN32
void Joystick::k_fillParams()
{
    if(rusWork == false)
    {
        _params.rusParams.x = 0.0;
        _params.rusParams.y = 0.0;
        _params.rudParams.z = 0.0;
    }


}
#endif
