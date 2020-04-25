#include <Ultralight/platform/FontLoader.h>

namespace ultralight {

/**
 * FontLoader implementation that just returns an embedded font (Roboto).
 */
class FontLoaderRoboto : public FontLoader {
public:
  FontLoaderRoboto() {}
  virtual ~FontLoaderRoboto() {}
  String16 fallback_font() const override;
  String16 fallback_font_for_characters(const String16& characters, int weight, bool italic, float size) const override;
  Ref<Buffer> Load(const String16& family, int weight, bool italic, float size) override;
};

}  // namespace ultralight


