//+--------------------------------------------------------------------------
//
// File:        StarEffect.h
//
// NightDriverStrip - (c) 2018 Plummer's Software LLC.  All Rights Reserved.  
//
// This file is part of the NightDriver software project.
//
//    NightDriver is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//   
//    NightDriver is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//   
//    You should have received a copy of the GNU General Public License
//    along with Nightdriver.  It is normally found in copying.txt
//    If not, see <https://www.gnu.org/licenses/>.
//
// Description:
//
//   An effect that twinkles stars in and out.  Adapted from my NightDriver
//   code.
//
// History:     Nov-6-2018         Davepl      Commented
//
//---------------------------------------------------------------------------

#pragma once

#include "particles.h"

extern AppTime g_AppTime;

const int cMaxNewStarsPerFrame = 144;
const int cMaxStars = 500;
const int starWidth = 1;


class Star : public MovingFadingPaletteObject, public ObjectSize
{
  public:

    virtual double GetStarSize()
    {
        return _objectSize;
    }

    Star(const CRGBPalette16 & palette, TBlendType blendType = NOBLEND, double maxSpeed = 1.0, double starSize = 1.0)
        : MovingFadingPaletteObject(palette, blendType, maxSpeed),
          ObjectSize(starSize)
    {
    }
};

class RandomPaletteColorStar : public MovingFadingPaletteObject, public ObjectSize
{
  public:

    virtual double GetStarSize()
    {
        return _objectSize;
    }

    RandomPaletteColorStar(const CRGBPalette16 & palette, TBlendType blendType = NOBLEND, double maxSpeed = 1.0, double starSize = 1.0)
        : MovingFadingPaletteObject(palette, blendType, maxSpeed, random(16)*16),
          ObjectSize(starSize)
    {
    }
};

class LongLifeSparkleStar : public MovingFadingPaletteObject, public ObjectSize
{
    virtual float PreignitionTime() const         { return .25f;  }
    virtual float IgnitionTime()    const         { return 5.0f;  }
    virtual float HoldTime()        const         { return 0.00f; }
    virtual float FadeTime()        const         { return 0.0f;  }

  public:

    virtual double GetStarSize()
    {
        return _objectSize;
    }

    LongLifeSparkleStar(const CRGBPalette16 & palette, TBlendType blendType = NOBLEND, double maxSpeed = 1.0, double starSize = 1.0)
        : MovingFadingPaletteObject(palette, blendType, maxSpeed),
          ObjectSize(starSize)
    {
    }
};

class ColorStar : public MovingFadingColoredObject, public ObjectSize
{
  public:

    virtual double GetStarSize()
    {
        return _objectSize;
    }

    ColorStar(CRGB color, double maxSpeed = 1.0, double starSize = 1.0)
        : MovingFadingColoredObject(color, maxSpeed),
          ObjectSize(starSize)
    {
    }
};

class QuietStar : public RandomPaletteColorStar
{
  public:

    QuietStar(const CRGBPalette16 & palette, TBlendType blendType = NOBLEND, double maxSpeed = 10.0, double starSize = 1)
      : RandomPaletteColorStar(palette, blendType, maxSpeed, starSize)
    {}
        
    virtual float PreignitionTime() const { return 1.0f; }
    virtual float IgnitionTime()    const { return 0.00f; }
    virtual float HoldTime()        const { return 0.00f; }
    virtual float FadeTime()        const { return 2.0f;  }
    virtual float StarSize()        const { return 1;     }    
};

#if ENABLE_AUDIO
class MusicStar : public Star
{
    static float _baseHue;

  public:

    MusicStar(const CRGBPalette16 & palette, TBlendType blendType = NOBLEND, double maxSpeed = 2.0, double starSize = 1)
      : Star(palette, blendType, maxSpeed, starSize)
    {        
    }

    virtual float PreignitionTime() const      { return 0.0f; }
    virtual float IgnitionTime()    const      { return 0.0 ; }
    virtual float HoldTime()        const      { return 0.0f; }
    virtual float FadeTime()        const      { return 0.5f;  }

};
float MusicStar::_baseHue = 0.0f;


class MusicPulseStar : public Star
{
    public:

    MusicPulseStar(const CRGBPalette16 & palette, TBlendType blendType = LINEARBLEND, double maxSpeed = 0.0, double size = 0.0)
      : Star(palette, blendType, maxSpeed, size)
    {

    }

    virtual ~MusicPulseStar()
    {
    }

    virtual float PreignitionTime() const { return 0.00f;  }
    virtual float IgnitionTime()    const { return 0.00f; }
    virtual float HoldTime()        const { return 1.00f;  }
    virtual float FadeTime()        const { return 2.00f; } 
    virtual double GetStarSize()    const { return 1 + _objectSize * g_Analyzer._VURatio; }
};

#endif

class BubblyStar : public Star
{
    protected:
    int         _hue;

    public:
    
    BubblyStar(const CRGBPalette16 & palette, TBlendType blendType = LINEARBLEND, double maxSpeed = 2.0, double starSize = 12)
      : Star(palette, blendType, maxSpeed, starSize)
    {
        static float lastHue = 0;
        _hue        = lastHue;
        lastHue += 16;
        lastHue = fmod(lastHue, 256);
    }

    virtual double GetStarSize()
    {
        float x = Age()/TotalLifetime();
        float ratio1 = -1 * (2*(x-.5)) * (2*(x-.5)) + 1;
        float width = ratio1 * _objectSize;
        return width;
    }

    virtual ~BubblyStar()
    {}

    virtual float PreignitionTime() const  { return 0.00f; }
    virtual float IgnitionTime()    const  { return 0.05f; }
    virtual float HoldTime()        const  { return 0.25f; }
    virtual float FadeTime()        const  { return 0.50f; }
};

class FlashStar : public Star
{
    using Star::Star;

    virtual float PreignitionTime() const  { return 0.00f; }
    virtual float IgnitionTime()    const  { return 0.10f; }
    virtual float HoldTime()        const  { return 0.10f; }
    virtual float FadeTime()        const  { return 0.05f; }
};

class ColorCycleStar : public Star
{
    protected:
    int         _brightness;

    public:

    ColorCycleStar(const CRGBPalette16 & palette, TBlendType blendType = LINEARBLEND, double maxSpeed = 2.0, int speedDivisor = 1)
      : Star(palette, blendType, maxSpeed)
    {
        _brightness = randomDouble(128,255);
    }

    virtual CRGB Render(TBlendType blend)
    {
        CRGB c = ColorFromPalette(_palette, millis() / 2048.0f, _brightness, blend);
        fadeToBlackBy(&c, 1, 255 * FadeoutAmount());
        return c;        
    }
    
    virtual ~ColorCycleStar()
    {}
    
    virtual float PreignitionTime() const { return 2.0f; }
    virtual float IgnitionTime()    const { return 0.0f;}
    virtual float HoldTime()        const { return 2.00f; }
    virtual float FadeTime()        const { return 0.5f;  }
    virtual float StarSize()        const { return 1;     }
};

class MultiColorStar : public Star
{
    protected:
    uint8_t         _brightness;
    uint8_t         _hue;

    public:
    MultiColorStar(const CRGBPalette16 & palette, TBlendType blendType = LINEARBLEND, double maxSpeed = 2.0, int speedDivisor = 1)
      : Star(palette, blendType, maxSpeed)
    {
        _brightness = randomDouble(128,255);
        _hue        = randomDouble(0, 255);
    }

    virtual CRGB Render(TBlendType blend)
    {
        CRGB c = ColorFromPalette(_palette, _hue, _brightness, blend);
        fadeToBlackBy(&c, 1, 255 * FadeoutAmount());
        return c;        
    }
    
    virtual ~MultiColorStar()
    {}

    virtual float PreignitionTime() const { return 2.0f; }
    virtual float IgnitionTime()    const { return 0.0f; }
    virtual float HoldTime()        const { return 2.00f;}
    virtual float FadeTime()        const { return 0.5f; }
    virtual float StarSize()        const { return 1;    }
};

class ChristmasLightStar : public Star
{
  public:

    ChristmasLightStar(const CRGBPalette16 & palette, TBlendType blendType, double maxSpeed = 0.0)
      : Star(palette, blendType, maxSpeed, 1.0)

    {
        int iColor = randomDouble(0,255);
        _colorIndex = iColor;
    }

    virtual ~ChristmasLightStar()
    {}

    virtual float PreignitionTime() const { return 0.20f; }
    virtual float IgnitionTime()    const { return 0.00f; }
    virtual float HoldTime()        const { return 6.0f;  }
    virtual float FadeTime()        const { return 1.25f; }    
    virtual float StarSize()        const { return 0.00f; }
};

// Hot white stars that cool down through white, yellow, red

class HotWhiteStar : public Star
{
  public:

    HotWhiteStar(const CRGBPalette16 & palette, TBlendType blendType = LINEARBLEND, double maxSpeed = 0.0, double size = 0.0)
      : Star(palette, blendType, maxSpeed, size)
    {
    }

    virtual ~HotWhiteStar()
    {}

    virtual float PreignitionTime() const { return 0.00f;  }
    virtual float IgnitionTime()    const { return 0.20f;  }
    virtual float HoldTime()        const { return 0.00f;  }
    virtual float FadeTime()        const { return 2.00f;  }  

    virtual CRGB RenderColor(TBlendType blend)
    {
        if (Age() < IgnitionTime() + HoldTime())
            return CRGB::White;
        CRGB c = ColorFromPalette(_palette, 130*(1.0f-FadeoutAmount()), 256*(1.0f-FadeoutAmount()), blend);
        return c;
    }      
};



/*
template <typename ObjectType> class BeatStarterEffect : public BeatEffectBase
{
  protected:

    uint16_t                        _maxSize;

  public:

    BeatStarterEffect<ObjectType>(uint16_t )

    virtual void HandleBeat(bool bMajor, float elapsed, double span)
    {
        ObjectType newstar(_palette, _blendType, _maxSpeed * _musicFactor, _starSize);
        // This always starts stars on even pixel boundaries so they look like the desired width if not moving
        newstar._iPos = (int) randomDouble(0, _cLEDs-1-starWidth);
        _allParticles.push_back(newstar);

    }

    virtual void Draw()
    {
        BeatEffectBase::Draw();
    }
};

*/

// StarryNightEffect template 
//
// Generates up to 

template <typename StarType> class StarryNightEffect : public LEDStripEffect
{
  protected:
    std::deque<StarType>         _allParticles;
    const CRGBPalette16         _palette;
    float                        _newStarProbability;
    float                        _starSize;
    const TBlendType             _blendType;
    double                       _maxSpeed;
    double                       _blurFactor;
    double                       _musicFactor;
    CRGB                         _skyColor;

  public:


    StarryNightEffect<StarType>(const String & strName,
                                const CRGBPalette16& palette, 
                                float probability = 1.0, 
                                float starSize = 1.0, 
                                TBlendType blendType = LINEARBLEND, 
                                double maxSpeed = 100.0,
                                double blurFactor = 0.0,
                                double musicFactor = 1.0,
                                CRGB skyColor = CRGB::Black)
      : LEDStripEffect(strName),
        _palette(palette),
        _newStarProbability(probability),
        _starSize(starSize),
        _blendType(blendType),
        _maxSpeed(maxSpeed),
        _blurFactor(blurFactor),
        _musicFactor(musicFactor),
        _skyColor(skyColor)
    {
    }

    virtual float StarSize()
    {
        return _starSize;
    }

    virtual void Clear()
    {
        LEDStripEffect::setAllOnAllChannels(_skyColor.r, _skyColor.g, _skyColor.b);
    }

    virtual void Draw()
    {
        if (_blurFactor == 0)
        {
            Clear();
        }
        else
        {
            blurRows(graphics()->leds, MATRIX_WIDTH, MATRIX_HEIGHT, _blurFactor * 255);
            fadeAllChannelsToBlackBy(40 * (2.0 - g_Analyzer._VURatioFade));
        }
        Update();
        CreateStars();
    }

    virtual void CreateStars()
    {
        for (int i = 0; i < cMaxNewStarsPerFrame; i++)
        {
            float prob = _newStarProbability;

            #if ENABLE_AUDIO
                if (_musicFactor != 1.0)
                {
                    // 
                    prob = prob * (g_Analyzer._VURatio - 0.5) * _musicFactor; 
                }
            #endif

            if (randomDouble(0, 1.0) < g_AppTime.DeltaTime() * prob * (float) _cLEDs / 5000.0f)
            {
                //Serial.printf("Creating star with speed = %lf and factor = %lfn", _maxSpeed, _musicFactor);
                StarType newstar(_palette, _blendType, _maxSpeed * _musicFactor, _starSize);
                // This always starts stars on even pixel boundaries so they look like the desired width if not moving
                newstar._iPos = (int) randomDouble(0, _cLEDs-1-starWidth);
                _allParticles.push_back(newstar);
            }
        }
    }

    virtual void Update()
    {
        for(auto i = _allParticles.begin(); i != _allParticles.end(); i++)
        {
            i->UpdatePosition();
            float fPos = i->_iPos;
            CRGB c = i->ObjectColor();
            graphics()->setPixelsF(fPos - i->_objectSize / 2.0, i->_objectSize, c, true);         
        }

        while (_allParticles.size() > 0 && _allParticles.front().Age() >= _allParticles.front().TotalLifetime())
            _allParticles.pop_front();

        while (_allParticles.size() > cMaxStars)
            _allParticles.pop_front();
    }

};

template <typename StarType> class BlurStarEffect : public StarryNightEffect<StarType>
{
  private:

  public:

    BlurStarEffect<StarType>(const CRGBPalette16 & palette, float probability = 0.2, size_t starSize = 1, TBlendType blendType = LINEARBLEND, double maxSpeed = 20.0)
        : StarryNightEffect<StarType>(palette, probability, starSize, blendType, maxSpeed)
    {
    }

    virtual void Clear()
    {
        StarryNightEffect<StarType>::setAll(32,0,0);
    }
};

// TwinkleStarEffect
//
// Twinkles random colored dots on and off

class TwinkleStarEffect : public LEDStripEffect
{
    #define NUM_TWINKLES 100
    int buffer[NUM_TWINKLES];

public:

    TwinkleStarEffect() : LEDStripEffect("Twinkle Star")
    {

    }

    virtual bool Init(std::shared_ptr<GFXBase> gfx[NUM_CHANNELS])
    {
        LEDStripEffect::Init(gfx);
        for (int i = 0; i < NUM_TWINKLES; i++)
            buffer[i] = -1;      
        return true;    
    }

    virtual void Draw()
    {

        // Init all the memory slots to -1 which means "empty slot"


        // Rotate the buffer
        //memmove(buffer, buffer + 1, ARRAYSIZE(buffer) * (Count - 1));
        for (int i = 0; i < NUM_TWINKLES - 1; i++)
            buffer[i] = buffer[i + 1];

        // If we had a valid pixel in slot 0, we can blank it now

        if (buffer[0] >= 0)
            setPixelsOnAllChannels(buffer[0], 0, 0, 0);

        // Pick a random pixel and put it in the TOP slot
        int iNew = (int) randomDouble(0, _cLEDs);
        setPixelOnAllChannels(iNew, RandomRainbowColor());
        buffer[NUM_TWINKLES - 1] = iNew;
    }
};
