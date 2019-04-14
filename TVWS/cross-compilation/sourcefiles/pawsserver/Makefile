#
# Top level makefile for simple application
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pawsserver
PKG_VERSION:=0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/pawsserver
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=pawsserver -- PAWS Client
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

TARGET_CFLAGS += $(FPIC)

define Package/pawsserver/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pawsserver $(1)/bin/
endef

$(eval $(call BuildPackage,pawsserver))
