
String macToString(const byte* mac) {
  char buf[13];
  memset(buf, 0, 13);
  snprintf(buf, 13, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

