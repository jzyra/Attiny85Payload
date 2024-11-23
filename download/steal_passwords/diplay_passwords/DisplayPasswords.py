#!/usr/bin/python3

import json
import sqlite3
import base64
import sys
import os
import xml.etree.ElementTree as ET
from Cryptodome.Cipher import AES

class DisplayPasswords:

  @staticmethod
  def __load_file(file_passwords):
    data = None
    with open(file_passwords, 'r') as file:
      data = json.load(file)
    return data

  @staticmethod
  def __decrypt_passwords(key, encoded_password):
    res = None
    decoded_key = base64.b64decode(key)
    cipher_text = base64.b64decode(encoded_password)
    res = cipher_text
    try:
      initialisation_vector = cipher_text[3:15]
      encrypted_password = cipher_text[15:-16]
      cipher = AES.new(decoded_key, AES.MODE_GCM, initialisation_vector)
      res = cipher.decrypt(encrypted_password).decode()
    except Exception as e:
      pass
    return res

  @staticmethod
  def __get_all_keys(obj):
    return obj["chrome"]["keys"]

  @staticmethod
  def __get_all_contents(obj):
    return obj["chrome"]["contents"]

  @staticmethod
  def __get_contents_wifi(obj):
    return obj["wifi"]

  @staticmethod
  def __get_name_ssid(xml):
    names = xml.findall('.//{*}name')
    if len(names) > 0:
      return names[0].text
    return None

  @staticmethod
  def __get_password_ssid(xml):
    passwords = xml.findall('.//{*}MSM/{*}security/{*}sharedKey/{*}keyMaterial')
    if len(passwords) > 0:
      return passwords[0].text
    return None

  @staticmethod
  def __display_passwords_wifi(contents):
    for content in contents:
      xml = base64.b64decode(content)
      root = ET.fromstring(xml)
      print("ssid     : {0}".format(DisplayPasswords.__get_name_ssid(root)))
      print("password : {0}".format(DisplayPasswords.__get_password_ssid(root)))
      print("");

  @staticmethod
  def __display_passwords_chrome(keys, contents):
    for passwords in contents:
      for password in passwords:
        displayed_password = ""
        for key in keys:
          raw_password = DisplayPasswords.__decrypt_passwords(key, password["password"])
          if type(raw_password) is str:
            displayed_password = raw_password
            break
          else:
            displayed_password = ""
        print("url      : {0}".format(password["url"]))
        print("username : {0}".format(password["username"]))
        print("password : {0}".format(displayed_password))
        print("")

  @staticmethod
  def __check_args(args):
    if len(args) > 1:
      if os.access(args[1], os.F_OK | os.R_OK):
        return True
    return False

  @staticmethod
  def __print_help(args):
    print("Usage:")
    print("{0} JSON_FILE".format(args[0]))

  @staticmethod
  def main(args):
    if DisplayPasswords.__check_args(args) :
      data = DisplayPasswords.__load_file(args[1])
      DisplayPasswords.__display_passwords_chrome(DisplayPasswords.__get_all_keys(data), DisplayPasswords.__get_all_contents(data))
      DisplayPasswords.__display_passwords_wifi(DisplayPasswords.__get_contents_wifi(data))
    else:
      DisplayPasswords.__print_help(args)

if __name__ == '__main__':
  DisplayPasswords.main(sys.argv)
