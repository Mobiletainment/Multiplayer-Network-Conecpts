//!---------------------------------------------------------------------------
//! \file "CCControlBase.cpp"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "stdafx.h"
#include "CCControlBase.h"

namespace sl	{

	CCControlBase::CCControlBase()
		:_backGroundSprite(nullptr)
		,_dataSource(nullptr)
		,m_marginH(10)	
		,m_marginV(5)
	{
		_valueKey = new CCString();
	}

	CCControlBase::~CCControlBase()	{
		setBackGroundSprite(nullptr);
		setDataSource(nullptr);

		CC_SAFE_RELEASE_NULL(_valueKey);
	}

	void CCControlBase::visit()	{
		if(getCtrlFlags().hasFlag(ECtrlFlag_UseScissorTest))	{
			_scissorRestored = false;
			CCRect frame = getWorldViewRect();
			CCRect parentScissorRect;

			if (CCEGLView::sharedOpenGLView()->isScissorEnabled()) {
				_scissorRestored = true;
				parentScissorRect = CCEGLView::sharedOpenGLView()->getScissorRect();
				CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
			}
			else 	{
				glEnable(GL_SCISSOR_TEST);
				CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
			}

			SLBaseClass::visit();

			if (_scissorRestored) {
				// restore the parent's scissor rect
				CCEGLView::sharedOpenGLView()->setScissorInPoints(parentScissorRect.origin.x, parentScissorRect.origin.y, parentScissorRect.size.width, parentScissorRect.size.height);
			}
			else 	{
				glDisable(GL_SCISSOR_TEST);
			}
		}
		else	{
			SLBaseClass::visit();
		}
	}

	void CCControlBase::draw()	{
		SLBaseClass::draw();
		if(getCtrlFlags().hasFlag(ECtrlFlag_DrawFocusRect))	{
			if(isSelected())	{
				drawFrame(ccc4(255,0,0,127),5);
			}
			else	{
				drawFrame(ccc4(0,127,200,127),5);
			}
		}
	}	

	CCRect CCControlBase::getWorldViewRect()	{
		const CCSize& contentSize(getContentSize());
		CCPoint position = getPosition();
		position = this->getParent()->convertToWorldSpace(position);
		return CCRectMake(position.x, position.y, contentSize.width, contentSize.height);
	}

	void CCControlBase::onAppResized(CCObject* app)	{
		CCAssert(false,"implement this");
	}

	bool CCControlBase::init()	{
		CCScale9Sprite* backgroundSprite(nullptr);
		return initWithBackGroundSprite(backgroundSprite);
	}

	bool CCControlBase::initWithBackGroundSprite( CCScale9Sprite* backgroundSprite )	{
		bool initialized(SLBaseClass::init());
		if(initialized)		{
			if(backgroundSprite != nullptr)	{
				setBackGroundSprite(backgroundSprite);
				addChild(backgroundSprite,-1);
			}
			addSubLayerContent();
			needsLayout();
		}

		return initialized;
	}

	bool CCControlBase::initWithBackGroundSprite( const char* backgroundSpriteName )	{
		CCScale9Sprite* backgroundSprite(nullptr);
		if(isEmptyString(backgroundSpriteName) == false)	{
			backgroundSprite = CCScale9Sprite::create(backgroundSpriteName);
		}
		return initWithBackGroundSprite(backgroundSprite);
	}

	void CCControlBase::setMargins(int marginH, int marginV)	{
		m_marginV = marginV;
		m_marginH = marginH;
		needsLayout();
	}

	void CCControlBase::redoLayout() {
		// intentionally empty
		// see implementation in CCControlContainer
	}


	void CCControlBase::needsLayout()	{
		CCSize ctrlSize(getPreferredSize());
		if(getBackGroundSprite() == nullptr)	{
			if(ctrlSize.width == 0)	{
				ctrlSize.width = 100;
			}
			if(ctrlSize.height == 0)	{
				ctrlSize.height = 100;
			}
		}	
		else	{
			ctrlSize = getBackGroundSprite()->getContentSize();
			if(getPreferredSize().width != 0)	{
				ctrlSize.width = getPreferredSize().width;
			}
			if(getPreferredSize().height != 0)	{
				ctrlSize.height = getPreferredSize().height;
			}
			if(ctrlSize.equals(getBackGroundSprite()->getContentSize()) == false)	{
				getBackGroundSprite()->setContentSize(ctrlSize);
			}

			const CCPoint& anchorPoint(getAnchorPoint());

			if(isIgnoreAnchorPointForPosition())	{
				getBackGroundSprite()->setPosition(
					ccp( ctrlSize.width * 0.5f, ctrlSize.height * 0.5f )
					);
			}
			else	{
				getBackGroundSprite()->setPosition(
					ccp( ctrlSize.width, ctrlSize.height )
					);
			}
		}
		setContentSize(ctrlSize);
	}

	void CCControlBase::layoutChildren(ELayoutMode layoutMode, bool resize)	{
		// now check for all sub layers and call needsLayout to
		CCArray* children(getChildren());
		CCObject* child;

		const float verticalOffset(static_cast<float>(m_marginV));


		if(resize == true)	{

			const CCSize& currentContentSize(getContentSize());
			float contentHeight(verticalOffset);
			// determine the new content height
			CCARRAY_FOREACH(children, child)
			{
				CCControlBase* ctrlBase(dynamic_cast<CCControlBase*>(child));
				if(ctrlBase != nullptr)	{
					CCSize preferredSize(CCSizeMake(currentContentSize.width - (m_marginH * 2),0));
					CCSize ctrlPreferredSize(ctrlBase->getPreferredSize());
					ctrlPreferredSize.width = preferredSize.width;
					ctrlBase->setPreferredSize(ctrlPreferredSize);
					ctrlBase->needsLayout();

					contentHeight += ctrlBase->getContentSize().height;
					contentHeight += verticalOffset;
				}
				else	{
					CCScale9Sprite* sprite(dynamic_cast<CCScale9Sprite*>(child));
					if(sprite == _backGroundSprite)	{

					}
					else	{
						CCControlButton* btn(dynamic_cast<CCControlButton*>(child));
						if(btn != nullptr)	{
							contentHeight += (btn->getContentSize().height + verticalOffset * 2);
							contentHeight += verticalOffset * 2;
						}
						else	{
							CCNode* node(dynamic_cast<CCNode*>(child));
							if(node != nullptr)	{
								contentHeight += node->getContentSize().height;
								contentHeight += verticalOffset;
							}
						}
					}
				}
			}

			CCSize newContentSize(getContentSize());
			newContentSize.height = contentHeight;
			setPreferredSize(newContentSize);
			needsLayout();
			layoutChildren(ELayoutMode_topLeftDown);
		}

		const CCSize& contentSize(getContentSize());
		// top down layout
		CCPoint ctrlPosition(
			static_cast<float>(m_marginH),
			getContentSize().height - static_cast<float>(m_marginV));

		CCARRAY_FOREACH(children, child)
		{
			CCControlBase* ctrlBase(dynamic_cast<CCControlBase*>(child));
			if(ctrlBase != nullptr)	{
				CCSize preferredSize(CCSizeMake(contentSize.width - (m_marginH * 2),0));
				CCSize ctrlPreferredSize(ctrlBase->getPreferredSize());
				ctrlPreferredSize.width = preferredSize.width;
				ctrlBase->setPreferredSize(ctrlPreferredSize);
				ctrlBase->needsLayout();

				ctrlPosition.y -= ctrlBase->getContentSize().height;
				ctrlBase->setPosition(ctrlPosition);
				ctrlPosition.y -= verticalOffset;
			}
			else	{
				CCScale9Sprite* sprite(dynamic_cast<CCScale9Sprite*>(child));
				if(sprite == _backGroundSprite)	{

				}
				else	{
					CCControlButton* btn(dynamic_cast<CCControlButton*>(child));
					if(btn != nullptr)	{
						ctrlPosition.y -= (btn->getContentSize().height + verticalOffset * 2);
						btn->setPosition(ctrlPosition);
						ctrlPosition.y -= verticalOffset * 2;
					}
					else	{
						CCNode* node(dynamic_cast<CCNode*>(child));
						if(node != nullptr)	{
							ctrlPosition.y -= node->getContentSize().height;
							node->setPosition(ctrlPosition);
							ctrlPosition.y -= verticalOffset;
						}
					}
				}
			}
		}
	}

	void CCControlBase::setContentSize(const CCSize& contentSize)	{
		SLBaseClass::setContentSize(contentSize);
	}

	CCObject* CCControlBase::getCtrlValue()	{
		if(_dataSource != nullptr)	{
			return _dataSource->objectForKey(getValueKey());
		}
		return nullptr;
	}

	void CCControlBase::setValueKey( const char* key )	{
		CCString* tmp(new CCString());
		tmp->initWithFormat("%s", key);
		*_valueKey = *tmp;
		tmp->release();
	}

	const char* CCControlBase::getValueKey() const	{
		return _valueKey->getCString();
	}

	bool CCControlBase::canSerializeTo( CCDictionary* target ) const	{
		if(isEmptyString(getValueKey()) == false)	{
			return (target != nullptr);
		}
		return false;
	}

	bool CCControlBase::canDeserializeFrom( CCDictionary* source ) const	{
		if(isEmptyString(getValueKey()) == false)	{
			return (source != nullptr);
		}
		return false;
	}

	void CCControlBase::serialize( CCDictionary* target )	{
		// this implementation only makes sense if you want to pass
		// values from one data source to another
		const char* valueKey(getValueKey());
		if(isEmptyString(valueKey) == false)	{
			if(target != getDataSource())	{
				CCObject* ctrlValue(getCtrlValue());
				if(ctrlValue != nullptr)	{
					target->setObject(ctrlValue,valueKey);
				}
			}
		}
	}

	// read from data source to ctrl value
	void CCControlBase::deserialize( CCDictionary* source )	{
		if(getDataSource() == nullptr)	{
			setDataSource(source);
		}
		const char* valueKey(getValueKey());
		if(isEmptyString(valueKey) == false)	{
			CCObject* ctrlValue(getCtrlValue());
			if(ctrlValue == nullptr)	{
				SL_PROCESS_APP()->log(ELogType_Warning, "failed to deserialize value key %s", valueKey);
			}
		}
		else	{
			// a control without value key will not serialize anything
		}
	}

	// write from ctrl value to datasource
	void CCControlBase::serializeAll( CCDictionary* target )	{
		serialize(target);
		CCArray* children(getChildren());
		if(children->count() > 0)	{
			CCObject* child;
			CCARRAY_FOREACH(children, child)
			{
				CCControlBase* ctrlLayer(dynamic_cast<CCControlBase*>(child));
				if(ctrlLayer != nullptr)	{
					ctrlLayer->serializeAll(target);
				}
			}
		}
	}

	void CCControlBase::deserializeAll( CCDictionary* source, bool reLayout )	{
		deserialize(source);
		CCArray* children(getChildren());
		if(children->count() > 0)	{
			CCObject* child;
			CCARRAY_FOREACH(children, child)
			{
				CCControlBase* ctrlLayer(dynamic_cast<CCControlBase*>(child));
				if(ctrlLayer != nullptr)	{
					ctrlLayer->deserializeAll(source, reLayout);
				}
			}
		}
		if(reLayout)	{
			redoLayout();
		}
	}

	void CCControlBase::addSubLayerContent()	{
		// empty implementation for now
	}

	void CCControlBase::onEnter()	{
		SLBaseClass::onEnter();
	}

	void CCControlBase::onExit()	{
		SLBaseClass::onExit();
	}

	bool CCControlBase::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )	{
		if (isTouchInside(pTouch) )
		{
//			CCLOG("CCControlBase::ccTouchBegan - isTouchInside");
		}
		if (!isEnabled() || !isVisible() || !hasVisibleParents() )
		{
//			CCLOG("CCControlBase::ccTouchBegan");
		}
		// return true if we claim this touch
		// so we will receive further touch events
		return false;
	}

	void CCControlBase::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )	{
//		CCLOG("CCControlBase::ccTouchEnded");
	}

	void CCControlBase::registerResizeObserver()	{
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->addObserver(this, callfuncO_selector(CCControlBase::onAppResized), SL_NOTIFY_APPRESIZED, nullptr );
	}

	void CCControlBase::deregisterResizeObserver()	{
		CCNotificationCenter* notificationCenter(CCNotificationCenter::sharedNotificationCenter());
		notificationCenter->removeObserver(this, SL_NOTIFY_APPRESIZED);
	}

	void CCControlBase::drawFrame( const ccColor4B& color, float lineWidth ) const	{

		const CCSize& contentSize(getContentSize());
		CCRect drawRect;
		drawRect.origin = CCPoint(0,0);
		drawRect.size = getContentSize();


		// line: color, width, aliased
		// glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
		// GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
		//	glDisable(GL_LINE_SMOOTH);
		lineWidth = sl::max(1.0f, lineWidth);
		glLineWidth( lineWidth);
		ccDrawColor4B(color.r, color.g, color.b, color.a);

		const float frameOffset(sl::max(0.5f, lineWidth * 0.5f));

		// 4 lines
		CCPoint line[4][2];

		line[0][0] = CCPoint(0,0);
		line[0][1] = CCPoint(0,contentSize.height);

		line[1][0] = line[0][1];
		line[1][1] = CCPoint(contentSize.width,contentSize.height);

		line[2][0] = line[1][1];
		line[2][1] = CCPoint(contentSize.width,0);

		line[3][0] = line[2][1];
		line[3][1] = line[0][0];

		line[0][0].x += frameOffset;
		line[0][1].x += frameOffset;

		line[1][0].y -= frameOffset;
		line[1][1].y -= frameOffset;

		line[2][0].x -= frameOffset;
		line[2][1].x -= frameOffset;

		line[3][0].y += frameOffset;
		line[3][1].y += frameOffset;

		ccDrawLine( line[0][0], line[0][1] );
		ccDrawLine( line[1][0], line[1][1] );
		ccDrawLine( line[2][0], line[2][1] );
		ccDrawLine( line[3][0], line[3][1] );
	}

	void CCControlBase::removeBackgroundSprite()
	{
		if(getBackGroundSprite() != nullptr)	{
			getBackGroundSprite()->removeFromParent();
			setBackGroundSprite(nullptr);
		}
	}

	
	CCControlBaseArray::CCControlBaseArray()
	{

	}

	CCControlBaseArray::~CCControlBaseArray()
	{

	}

	CCControlBase* CCControlBaseArray::getControlAt(SLSize idx) const	{
		if(idx < size())	{
			return (*this)[idx];
		}
		return nullptr;
	}

	void CCControlBaseArray::setSelected( bool value	)	{
		TCCControlBaseArray::const_iterator iterEnd(end());
		TCCControlBaseArray::iterator iter(begin());
		while(iter != iterEnd)	{
			(*iter)->setSelected(value);
			++iter;
		}
	}

	void CCControlBaseArray::add(CCControlBase* ctrl)	{
		TCCControlBaseArray::const_iterator ctrlIter(std::find (begin(), end(), ctrl));
		if(ctrlIter == end())	{
			push_back(ctrl);
		}
	}

	void CCControlBaseArray::remove(CCControlBase* ctrl)	{
		TCCControlBaseArray::const_iterator ctrlIter(std::find (begin(), end(), ctrl));
		if(ctrlIter != end())	{
			erase(ctrlIter);
		}
	}

}


