#pragma once
/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_ANIMATIONPLAYER.hpp
================================================================
*/
#include <vector>
#include <SFML/Graphics/Image.hpp>
namespace anim {
	class AnimationPlayer {
	private:
		std::vector<sf::Image> iImages;
		float fLength;
		bool bIsPlaying;
		float fT;

	public:
		void vSetLength(float l) {
			fLength = l;
		}
		float fGetLength() {
			return fLength;
		}
		void vSetT(float t) {
			fT = t;
		}
		float fGetT() {
			return fT;
		}
		void vSetIsPlaying(bool b) {
			bIsPlaying = b;
		}
		bool bGetIsPlaying() {
			return bIsPlaying;
		}
		sf::Image iGetCurrentImage() {
			unsigned int i = iImages.size() * fT / fLength;
			std::cout << i << std::endl;
			return iImages[i];
		}
		AnimationPlayer() {
			vSetLength(0);
			vSetIsPlaying(false);
			vSetT(0);
		}
		AnimationPlayer(std::vector<sf::Image>& i, float l) {
			vSetLength(l);
			iImages = i;
			vSetT(0);
			vSetIsPlaying(false);
		}
		void updateT(float fElapsedTime) {
			if (bGetIsPlaying()) {
				vSetT(fGetT() + fElapsedTime);
				if (fT > fLength) {
					vSetIsPlaying(false);
					vSetT(0);
				}
			}
		}
	};
}