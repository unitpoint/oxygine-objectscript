#include "oxygine-framework.h"
#include "res.h"
#include "Game.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameMenu.h"

using namespace oxygine;

#include <objectscript.h>
#include <os-binder.h>
#include <ext-datetime/os-datetime.h>

// ../objectscript/src/ext-datetime;

class OxygineOS: public ObjectScript::OS
{
public:

	OxygineOS()
	{
	}

	virtual bool isFileExist(const OS_CHAR * filename)
	{
		return file::exists(filename);
	}

	/* virtual int getFileSize(const OS_CHAR * filename)
	{
	}

	virtual int getFileSize(FileHandle * f)
	{
	} */
	
	virtual FileHandle * openFile(const OS_CHAR * filename, const OS_CHAR * mode)
	{
		return (FileHandle*)file::open(filename, mode);
	}

	virtual int readFile(void * buf, int size, FileHandle * f)
	{
		if(f){
			return file::read((file::handle)f, buf, (unsigned)size);
		}
		return 0;
	}

	virtual int writeFile(const void * buf, int size, FileHandle * f)
	{
		if(f){
			file::write((file::handle)f, buf, (unsigned)size);
		}
		return size;
	}

	virtual int seekFile(FileHandle * f, int offset, int whence)
	{
		if(f){
			return file::seek((file::handle)f, offset, whence);
		}
		return 0;
	}
	
	virtual void closeFile(FileHandle * f)
	{
		if(f){
			file::close((file::handle)f);
		}
	}

	virtual void echo(const void * buf, int size)
	{
		const char * output_filename = "out.txt";
		FileHandle * f = openFile(output_filename, "a");
		OS_ASSERT(f);
		writeFile((const char*)buf, size, f);
		closeFile(f);
	}
};

namespace ObjectScript {

template <class T> struct CtypeOxygineClass{};
template <class T> struct CtypeOxygineClass<T*>
{
	typedef typename RemoveConst<T>::type ttype;
	typedef typename RemoveConst<T>::type * type;

	static bool isValid(const type p){ return p != NULL; }
	static type def(ObjectScript::OS*){ return type(); }
	static type getArg(ObjectScript::OS * os, int offs){ return (type)os->toUserdata(CtypeId<ttype>::getInstanceId(), offs, CtypeId<ttype>::getId()); }
	static void push(ObjectScript::OS * os, const type val)
	{
		if(!val){
			os->pushNull();
			return;
		}
		// pushCtypeValue(os, val);
		os->pushUserPointer(CtypeId<ttype>::getInstanceId(), val, UserDataDestructor<ttype>::dtor);
		val->addRef();
		os->pushStackValue();
		os->getGlobal(CtypeName<ttype>::getName());
		if(!os->isUserdata(CtypeId<ttype>::getId(), -1)){
			os->pop(2);
		}else{
			os->setPrototype(CtypeId<ttype>::getInstanceId());
		}
	}
};

template <class T> struct CtypeOxygineSmartClass
{
	typedef typename T::element_type ttype;
	typedef typename T::element_type * type;

	static bool isValid(const type p){ return p != NULL; }
	static type def(ObjectScript::OS*){ return type(); }
	static type getArg(ObjectScript::OS * os, int offs){ return (type)os->toUserdata(CtypeId<ttype>::getInstanceId(), offs, CtypeId<ttype>::getId()); }
	static void push(ObjectScript::OS * os, const T& val)
	{
		if(!val){
			os->pushNull();
			return;
		}
		// pushCtypeValue(os, val);
		os->pushUserPointer(CtypeId<ttype>::getInstanceId(), val.get(), UserDataDestructor<ttype>::dtor);
		val->addRef();
		os->pushStackValue();
		os->getGlobal(CtypeName<ttype>::getName());
		if(!os->isUserdata(CtypeId<ttype>::getId(), -1)){
			os->pop(2);
		}else{
			os->setPrototype(CtypeId<ttype>::getInstanceId());
		}
	}
};

#define OS_DECL_OX_CLASS(type) OS_DECL_OX_CLASS_NAME(type, #type)
#define OS_DECL_OX_CLASS_NAME(type, name) \
	OS_DECL_CTYPE_NAME(type, name); \
	template <> struct CtypeValue<type*>: public CtypeOxygineClass<type*>{}; \
	template <> struct CtypeValue< intrusive_ptr<type> >: public CtypeOxygineSmartClass< intrusive_ptr<type> >{}; \
	template <> struct UserObjectDestructor<type>{ static void dtor(type * p){ p->releaseRef(); } };

// OS_DECL_OX_CLASS(oxygine::RootActor, "RootActor");

// OS_DECL_CTYPE(b2Vec2);
OS_DECL_CTYPE_NAME(Vector2, "vec2");

OS_DECL_OX_CLASS_NAME(Object, "OxygineObject");
OS_DECL_OX_CLASS(EventDispatcher);
OS_DECL_OX_CLASS(Actor);
OS_DECL_OX_CLASS(Sprite);
OS_DECL_OX_CLASS(RootActor);

// =====================================================================

template <>
struct CtypeValue<Vector2>
{
	typedef Vector2 type;

	static bool isValid(const Vector2&){ return true; }

	// static type to(const b2Vec2& val){ return (type)val; }
	static Vector2 def(ObjectScript::OS * os){ return Vector2(0, 0); }
	static Vector2 getArg(ObjectScript::OS * os, int offs)
	{
		if(os->isObject(offs)){
			float x = (os->getProperty(offs, "x"), os->popFloat());
			float y = (os->getProperty(offs, "y"), os->popFloat());
			return Vector2(x, y);
		}
#if 0
		if(os->isArray(offs)){
			os->pushStackValue(offs);
			os->pushNumber(0);
			os->getProperty();
			float x = os->popFloat();
		
			os->pushStackValue(offs);
			os->pushNumber(1);
			os->getProperty();
			float y = os->popFloat();

			return Vector2(x, y);
		}
#endif
		os->setException("vec2 required");
		return Vector2(0, 0);
	}

	static void push(ObjectScript::OS * os, const Vector2& p)
	{
#if 1
		os->getGlobal("vec2");
		os->pushGlobals();
		os->pushNumber(p.x);
		os->pushNumber(p.y);
		os->call(2, 1);
#else
		os->newObject();
	
		os->pushStackValue();
		os->pushNumber(p.x);
		os->setProperty("x", false);
				
		os->pushStackValue();
		os->pushNumber(p.y);
		os->setProperty("y", false);
#endif
	}
};

struct Oxygine
{
	static void registerObject(OS * os)
	{
		struct Lib {
			static int get(OS * os, int params, int, int, void*)
			{
				os->setException(OS::String::format(os, "property \"%s\" not found in \"%s\"", 
					os->toString(-params+0).toChar(),
					os->getValueNameOrClassname(-params-1).toChar()));
				return 0;
			}
		};

		OS::FuncDef funcs[] = {
			{"__get", &Lib::get},
			{}
		};
		registerUserClass<Object>(os, funcs);
	}

	static void registerEventDispatcher(OS * os)
	{
		struct Lib {
			void eventCallback(Event *ev)
			{
				// int funcId
			}

			static int addEventListener(OS * os, int params, int, int, void*)
			{
				OS_GET_SELF(EventDispatcher*);
				if(params < 2){
					os->setException("two arguments required");
					return 0;
				}
				if(!os->isFunction(-params+1)){
					os->setException("2nd argument must be function");
					return 0;
				}
				int funcId = os->getValueId(-params+1);
				eventType ev;
				switch(os->getType(-params+0)){
				case OS_VALUE_TYPE_NUMBER:
					ev = (eventType)os->toInt(-params+0);
					break;

				case OS_VALUE_TYPE_STRING:
					{
						OS::String name = os->toString(-params+0);
						ev = (eventType)name.string->hash;
						break;
					}

				default:
					os->setException("the first argument should be string or number");
					return 0;
				}
				self->addEventListener(ev, CLOSURE((Lib*)funcId, &Lib::eventCallback));
				return 0;
			}

			static int removeEventListener(OS * os, int params, int, int, void*)
			{
				OS_GET_SELF(Sprite*);
				return 0;
			}
		};

		OS::FuncDef funcs[] = {
			def("removeAllEventListeners", &EventDispatcher::removeAllEventListeners),
			def("__get@listenersCount", &EventDispatcher::getListenersCount),
			{"addEventListener", &Lib::addEventListener},
			{"removeEventListener", &Lib::removeEventListener},
			{}
		};
		registerUserClass<EventDispatcher, Object>(os, funcs);
	}

	static void registerActor(OS * os)
	{
		struct Lib // Actor: public oxygine::Actor
		{
			static Actor * __newinstance()
			{
				return new Actor();
			}
		};

		OS::FuncDef funcs[] = {
			def("__newinstance", &Lib::__newinstance),
			def("addChild", (void(Actor::*)(Actor*))&Actor::addChild),

			def("__get@x", &Actor::getX),
			def("__set@x", &Actor::setX),
			
			def("__get@y", &Actor::getY),
			def("__set@y", &Actor::setY),
			
			def("__get@width", &Actor::getWidth),
			def("__set@width", &Actor::setWidth),
			
			def("__get@height", &Actor::getHeight),
			def("__set@height", &Actor::setHeight),
			
			def("__get@priority", &Actor::getPriority),
			def("__set@priority", &Actor::setPriority),
			
			def("__get@anchor", &Actor::getAnchor),
			def("__set@anchor", (void(Actor::*)(const Vector2 &))&Actor::setAnchor),
			{}
		};
		registerUserClass<Actor, EventDispatcher>(os, funcs);
	}

	static void registerSprite(OS * os)
	{
		struct Lib // Actor: public oxygine::Actor
		{
			static Sprite * __newinstance()
			{
				return new Sprite();
			}

			static int setResAnimTest(OS * os, int params, int, int, void*)
			{
				OS_GET_SELF(Sprite*);
				OS::String name = os->toString(-params+0);
				self->setResAnim(res::ui.getResAnim(name.toChar()));
				return 0;
			}
		};

		OS::FuncDef funcs[] = {
			def("__newinstance", &Lib::__newinstance),
			{"setResAnimTest", &Lib::setResAnimTest},
			{}
		};
		registerUserClass<Sprite, Actor>(os, funcs);
	}

	static void registerRootActor(OS * os)
	{
		OS::FuncDef funcs[] = {
			{}
		};
		registerUserClass<RootActor, Actor>(os, funcs);
	}

	static OxygineOS * os;

	static void init()
	{
		os = OS::create(new OxygineOS());

		initDateTimeExtension(os);

		registerObject(os);
		registerEventDispatcher(os);
		registerActor(os);
		registerSprite(os);
		registerRootActor(os);
	}

	static void shutdown()
	{
		os->release();
	}

	static void run()
	{
		// pushCtypeValue(os, getRoot().get());
		pushCtypeValue(os, getRoot());
		os->setGlobal("root");

		os->require("main.os");
	}

}; // struct Oxygine

OxygineOS * Oxygine::os;

} // namespace ObjectScript

void example_preinit()
{
	ObjectScript::Oxygine::init();
}

void example_init()
{
	//load resources
	res::load();

	//create all scenes
	GameMenu::instance = new GameMenu;
	GameScene::instance = new GameScene;
	MainMenuScene::instance = new MainMenuScene;

	//show main menu
	MainMenuScene::instance->show();

	ObjectScript::Oxygine::run();
}

void example_update()
{

}

void example_destroy()
{
	MainMenuScene::instance = 0;
	GameScene::instance = 0;
	GameMenu::instance = 0;

	ObjectScript::Oxygine::shutdown();

	res::free();
}