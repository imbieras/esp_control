include $(TOPDIR)/rules.mk

PKG_NAME:=esp_control
PKG_RELEASE:=1
PKG_VERSION:=0.1.0
PKG_LICENSE:=GPL-3.0-only
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/esp_control
	CATEGORY:=Utilities
	TITLE:=esp_control
	DEPENDS:= +libserialport +libubus +libubox +libblobmsg-json
endef

define Package/esp_control/description
	Simple package for controlling Arduino based ESP8266 microcontrollers over serial 
endef

define Package/esp_control/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp_control $(1)/usr/bin
	$(INSTALL_BIN) ./files/esp_control.init $(1)/etc/init.d/esp_control
endef

$(eval $(call BuildPackage,esp_control))
