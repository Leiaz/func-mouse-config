#include "mouse_config_xml.h"

MouseConfigXml::MouseConfigXml()
{
}

bool MouseConfigXml::loadFile(const std::string &filename)
{
  return m_document.LoadFile(filename);
}

bool MouseConfigXml::saveToFile(const std::string &filename)
{
  return m_document.SaveFile(filename);
}

std::shared_ptr<MouseProfile> MouseConfigXml::extractProfile()
{
  std::shared_ptr<MouseProfile> profile(new MouseProfile);
  TiXmlHandle doc_handle(&m_document);
  TiXmlElement* elem;
  std::string name;
  int ret;

  elem = doc_handle.FirstChildElement("profile").ToElement();
  if (!elem) return nullptr;
  TiXmlHandle profile_handle(elem);
  ret = elem->QueryStringAttribute("name", &name);
  if (ret == TIXML_SUCCESS)
  {
    profile->setName(name);
  }
  //DPI
  elem = profile_handle.FirstChildElement("dpi_settings")
    .FirstChildElement().ToElement();
  for(; elem; elem=elem->NextSiblingElement())
  {
    int setting,value;
    // Just ignores "bad" elements for now.
    if (elem->ValueStr() != "dpi") break;
    ret = elem->QueryIntAttribute("setting",&setting);
    if (ret != TIXML_SUCCESS) break;
    ret = elem->QueryIntAttribute("value",&value);
    if (ret != TIXML_SUCCESS) break;
    profile->setDPI(static_cast<MouseProfile::DPI>(setting), value);
  }
  //LED
  elem = profile_handle.FirstChildElement("led_settings")
    .FirstChildElement().ToElement();
  for(; elem; elem=elem->NextSiblingElement())
  {
    if (elem->ValueStr() != "led") break;
    int setting,color,state;
    ret = elem->QueryIntAttribute("setting",&setting);
    if (ret != TIXML_SUCCESS) break;
    ret = elem->QueryIntAttribute("color",&color);
    if (ret == TIXML_SUCCESS)
    {
      profile->setLEDColor(static_cast<MouseProfile::LED>(setting), color);
    }
    ret = elem->QueryIntAttribute("state",&state);
    if (ret == TIXML_SUCCESS)
    {
      profile->setLEDState(static_cast<MouseProfile::LED>(setting),
          static_cast<MouseProfile::LEDState>(state));
    }
  }

  //Angle snapping
  {
    int value;
    elem = profile_handle.FirstChildElement("angle_snapping").ToElement();
    if (elem)
    {
      ret = elem->QueryIntAttribute("value",&value);
      if (ret == TIXML_SUCCESS)
      {
        profile->setAngleSnapping(value);
      }
    }
  }

  //Lift off distance
  {
    int value;
    elem = profile_handle.FirstChildElement("lift_off_distance")
      .ToElement();
    if (elem)
    {
      ret = elem->QueryIntAttribute("value",&value);
      if (ret == TIXML_SUCCESS)
      {
        profile->setLiftOffDistance(value);
      }
    }
  }

  //Buttons
  elem = profile_handle.FirstChildElement("button_settings")
    .FirstChildElement().ToElement();
  for(;elem;elem=elem->NextSiblingElement())
  {
    int button;
    if (elem->ValueStr() != "button") break;
    if (!elem->FirstChildElement()) break;
    ret = elem->QueryIntAttribute("id",&button);
    if (ret == TIXML_SUCCESS)
    {
      profile->setButton(static_cast<MouseProfile::MouseButton>(button),
          parseButtonSettingElement(elem->FirstChildElement()));
    }
  }
  return profile;
}

void MouseConfigXml::writeProfile(const MouseProfile &profile)
{
  TiXmlElement *elem;
  TiXmlElement *profile_elem = new TiXmlElement("profile");
  profile_elem->SetAttribute("name", profile.getName());
  m_document.LinkEndChild(profile_elem);

  //DPI
  elem = new TiXmlElement("dpi_settings");
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPIOneX));
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPIOneY));
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPITwoX));
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPITwoY));
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPIThreeX));
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPIThreeY));
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPIInstantAimX));
  elem->LinkEndChild(getDPIElement(profile,MouseProfile::DPIInstantAimY));
  profile_elem->LinkEndChild(elem);

  //LED
  elem = new TiXmlElement("led_settings");
  elem->LinkEndChild(getLEDElement(profile,
        MouseProfile::SideLED));
  elem->LinkEndChild(getLEDElement(profile,
        MouseProfile::WheelLED));
  profile_elem->LinkEndChild(elem);

  // Angle snapping
  elem = new TiXmlElement("angle_snapping");
  elem->SetAttribute("value", profile.getAngleSnapping());
  profile_elem->LinkEndChild(elem);

  // Lift off distance
  elem = new TiXmlElement("lift_off_distance");
  elem->SetAttribute("value", profile.getLiftOffDistance());
  profile_elem->LinkEndChild(elem);

  // Buttons
  elem = new TiXmlElement("button_settings");
  for (int i = 0;i<BUTTON_COUNT; i++)
  {
    TiXmlElement *button_elem = new TiXmlElement("button");
    button_elem->SetAttribute("id", i);
    button_elem->LinkEndChild(getButtonSettingElement(
          profile.getButtonSetting(
            static_cast<MouseProfile::MouseButton>(i))));
    elem->LinkEndChild(button_elem);
  }
  profile_elem->LinkEndChild(elem);
}

TiXmlElement *MouseConfigXml::getDPIElement(const MouseProfile &profile,
    MouseProfile::DPI s)
{
  TiXmlElement *elem = new TiXmlElement("dpi");
  elem->SetAttribute("setting", s);
  elem->SetAttribute("value", profile.getDPI(s));
  return elem;
}

TiXmlElement *MouseConfigXml::getLEDElement(const MouseProfile &profile,
    MouseProfile::LED led)
{
  TiXmlElement *elem = new TiXmlElement("led");
  elem->SetAttribute("setting", led);
  elem->SetAttribute("color", profile.getLEDColor(led));
  elem->SetAttribute("state", profile.getLEDState(led));
  return elem;
}

TiXmlElement *MouseConfigXml::getButtonSettingElement(
    const ButtonSetting &setting)
{
  TiXmlElement *elem = new TiXmlElement("button_setting");
  elem->SetAttribute("type",setting.getType());
  if (setting.getType() == ButtonSetting::Macro)
  {
    TiXmlElement *macro_elem = new TiXmlElement("macro");
    macro_elem->SetAttribute("name","noname");//TODO macro names
    for (auto macro_char : setting.getMacro())
    {
      TiXmlElement *macro_event = new TiXmlElement("macro_event");
      macro_event->SetAttribute("value",macro_char);
      macro_elem->LinkEndChild(macro_event);
    }
    elem->LinkEndChild(macro_elem);
  } else {
    elem->SetAttribute("value", setting.getValue());
  }
  return elem;
}

ButtonSetting MouseConfigXml::parseButtonSettingElement(TiXmlElement *elem)
{
  ButtonSetting setting;
  int type,value,ret;
  if (elem->ValueStr() != "button_setting") return setting;
  ret = elem->QueryIntAttribute("type",&type);
  if (ret != TIXML_SUCCESS) return setting; //Return default value
  switch (type)
  {
    case ButtonSetting::MouseAction:
      ret = elem->QueryIntAttribute("value",&value);
      if (ret == TIXML_SUCCESS) setting.setMouseAction(value);
      break;
    case ButtonSetting::MediaButton:
      ret = elem->QueryIntAttribute("value",&value);
      if (ret == TIXML_SUCCESS) setting.setMediaButton(value);
      break;
    case ButtonSetting::CommonShortcut:
      ret = elem->QueryIntAttribute("value",&value);
      if (ret == TIXML_SUCCESS) setting.setShortcut(value);
      break;
    case ButtonSetting::SingleKey:
      ret = elem->QueryIntAttribute("value",&value);
      if (ret == TIXML_SUCCESS) setting.setSingleKey(value);
      break;
    case ButtonSetting::Macro:
      if(elem->FirstChildElement())
      {
        setting.setMacro(parseMacro(elem->FirstChildElement()));
      }
      break;
  }
  return setting;
}

std::vector<unsigned char> MouseConfigXml::parseMacro(TiXmlElement *elem)
{
  std::vector<unsigned char> macro;
  // TODO macro name
  if (elem->ValueStr() != "macro") return macro;
  TiXmlElement *event_elem = elem->FirstChildElement();
  for(;event_elem;event_elem = event_elem->NextSiblingElement())
  {
    int value,ret;
    if (event_elem->ValueStr() != "macro_event") return macro;
    ret = event_elem->QueryIntAttribute("value",&value);
    if (ret == TIXML_SUCCESS)
      macro.push_back(value);
  }
  return macro;
}
