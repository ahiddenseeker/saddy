#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include <resource/tree.h>
#include <resource/link.h>
#include <db/custom/customschemafile.h>
#include <db/custom/customobject.h>
#include <db/save.h>
#include <db/load.h>
#include <renderer.h>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct SadDbCustomObjectTest : tpunit::TestFixture
{
 public:
   SadDbCustomObjectTest() : tpunit::TestFixture(	   
	   TEST(SadDbCustomObjectTest::test_label),
	   TEST(SadDbCustomObjectTest::test_sprite2d),
	   TEST(SadDbCustomObjectTest::test_lost_prop_on_reload),
	   TEST(SadDbCustomObjectTest::test_change_prop_type_on_reload),	   
	   TEST(SadDbCustomObjectTest::test_reload_same_type)
    )
   {

   }

   void test_label()
   {
	   sad::Renderer r;
	   sad::resource::Tree* tree = new sad::resource::Tree();
	   tree->setStoreLinks(true);
	   tree->setRenderer(&r);
	   r.addTree("", tree);

	   sad::Vector<sad::resource::Error *> errors = tree->loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::TextureMappedFont\","
					"\"filename\": \"examples/game/font\","
					"\"name\"    : \"objects\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/label.json\""
				"}"				
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::db::custom::Object object;
	   object.setTreeName(&r, "");
	   object.setProperty("schema", sad::String("myobject"));
	   object.setProperty("prop1", 3);
	   object.setProperty("prop2", 4);
	   ASSERT_TRUE(object.canBeRendered());
	   ASSERT_TRUE(object.getProperty<int>("prop1").exists());
	   ASSERT_TRUE(object.getProperty<int>("prop2").exists());
   }

   void test_sprite2d()
   {
	   sad::Renderer r;
	   sad::resource::Tree* tree = new sad::resource::Tree();
	   tree->setStoreLinks(true);
	   tree->setRenderer(&r);
	   r.addTree("", tree);

	   sad::Vector<sad::resource::Error *> errors = tree->loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::TextureMappedFont\","
					"\"filename\": \"examples/game/font\","
					"\"name\"    : \"objects\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/sprite.json\""
				"}"				
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::db::custom::Object object;
	   object.setTreeName(&r, "");
	   object.setProperty("schema", sad::String("myobject"));
	   object.setProperty("prop1", 3);
	   object.setProperty("prop2", 4);
	   ASSERT_TRUE(object.canBeRendered());
	   ASSERT_TRUE(object.getProperty<int>("prop1").exists());
	   ASSERT_TRUE(object.getProperty<int>("prop2").exists());
   }

   void test_lost_prop_on_reload()
   {
	   sad::Renderer r;
	   sad::resource::Tree* tree = new sad::resource::Tree();
	   tree->setStoreLinks(true);
	   tree->setRenderer(&r);
	   r.addTree("", tree);

	   sad::Vector<sad::resource::Error *> errors = tree->loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/sprite.json\""
				"}"				
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::db::custom::Object object;
	   object.setTreeName(&r, "");
	   object.setProperty("schema", sad::String("myobject"));
	   object.setProperty("prop1", 3);
	   ASSERT_TRUE(object.getProperty<int>("prop1").exists());

	   rename("tests/db/sprite.json", "tests/db/sprite.json.tmp");
	   rename("tests/db/sprite_lost_prop.json", "tests/db/sprite.json");

	   errors = tree->root()->resource("myobject")->file()->reload(); 
	   count = (int)(errors.size());
	   sad::util::free(errors);

	   rename("tests/db/sprite.json", "tests/db/sprite_lost_prop.json");
	   rename("tests/db/sprite.json.tmp", "tests/db/sprite.json");

	   ASSERT_TRUE(count == 0);
	   ASSERT_TRUE(object.canBeRendered());
	   ASSERT_FALSE(object.getProperty<int>("prop1").exists());
   }

   void test_change_prop_type_on_reload()
   {
	   sad::Renderer r;
	   sad::resource::Tree* tree = new sad::resource::Tree();
	   tree->setStoreLinks(true);
	   tree->setRenderer(&r);
	   r.addTree("", tree);

	   sad::Vector<sad::resource::Error *> errors = tree->loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/sprite.json\""
				"}"				
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::db::custom::Object object;
	   object.setTreeName(&r, "");
	   object.setProperty("schema", sad::String("myobject"));
	   object.setProperty("prop1", 3);
	   ASSERT_TRUE(object.getProperty<int>("prop1").exists());

	   rename("tests/db/sprite.json", "tests/db/sprite.json.tmp");
	   rename("tests/db/sprite_change_type.json", "tests/db/sprite.json");

	   errors = tree->root()->resource("myobject")->file()->reload(); 
	   count = (int)(errors.size());
	   sad::util::free(errors);

	   rename("tests/db/sprite.json", "tests/db/sprite_change_type.json");
	   rename("tests/db/sprite.json.tmp", "tests/db/sprite.json");

	   ASSERT_TRUE(count == 0);
	   ASSERT_TRUE(object.canBeRendered());
	   ASSERT_FALSE(object.getProperty<int>("prop1").exists());
	   ASSERT_TRUE(object.getProperty<sad::String>("prop1").exists());
   }

   void test_reload_same_type()
   {
	   sad::Renderer r;
	   sad::resource::Tree* tree = new sad::resource::Tree();
	   tree->setStoreLinks(true);
	   tree->setRenderer(&r);
	   r.addTree("", tree);

	   sad::Vector<sad::resource::Error *> errors = tree->loadFromString(
		   "["
				"{"
				    "\"type\"   : \"sad::resource::TextureAtlasFile\","
					"\"filename\": \"tests/icons.json\""
				"},"
				"{"
					"\"type\"   : \"sad::db::custom::SchemaFile\","
					"\"filename\": \"tests/db/sprite.json\""
				"}"				
			"]"
		);
	   int count = (int)(errors.size());
	   sad::util::free(errors);
	   ASSERT_TRUE(count == 0);

	   sad::db::custom::Object object;
	   object.setTreeName(&r, "");
	   object.setProperty("schema", sad::String("myobject"));
	   object.setProperty("prop1", 3);
	   ASSERT_TRUE(object.getProperty<int>("prop1").exists());


	   errors = tree->root()->resource("myobject")->file()->reload(); 
	   count = (int)(errors.size());
	   sad::util::free(errors);

	   ASSERT_TRUE(count == 0);
	   ASSERT_TRUE(object.canBeRendered());
	   ASSERT_TRUE(object.getProperty<int>("prop1").exists());
	   ASSERT_TRUE(object.getProperty<int>("prop1").value() == 3);
   }


} _sad_db_custom_object_test;