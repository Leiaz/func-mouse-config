#ifndef MOUSE_CONFIG_XML_H
#define MOUSE_CONFIG_XML_H

#include "mouse_profile.h"

#include <tinyxml.h>
#include <memory>
#include <string>

class MouseConfigXml
{
  public:
    MouseConfigXml();

    bool loadFile(const std::string &filename);
    bool saveToFile(const std::string &filename);

    /**
     * \return a pointer to a MouseProfile set to values contained in
     * the file or the default MouseProfile values for values that
     * are missing or can't be read for some reason (always returns a
     * usable profile).
     */
    std::shared_ptr<MouseProfile> extractProfile();
    void writeProfile(const MouseProfile &profile);

  private:
    TiXmlElement *getDPIElement(const MouseProfile &profile,
        MouseProfile::DPI s);
    TiXmlElement *getLEDElement(const MouseProfile &profile,
        MouseProfile::LED led);
    TiXmlElement *getButtonSettingElement(const ButtonSetting &setting);
    ButtonSetting parseButtonSettingElement(TiXmlElement *elem);
    std::vector<unsigned char> parseMacro(TiXmlElement *elem);

    TiXmlDocument m_document;
};
#endif
