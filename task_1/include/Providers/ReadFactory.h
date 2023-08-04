#pragma once
#include "Objects/Figures.h"
#include "DataProvider.h"

namespace Provider {

	class ReadFactory {
	private:
		ReadFactory(void) = default;
		ReadFactory(DataProvider<double>* dataprov);
		virtual ~ReadFactory(void) = default;

		friend class ObjProvider;

	public:
		virtual std::shared_ptr<Object> factory(const int type);
		double readFromData(bool& isDamaged);

		std::shared_ptr<Object> greateRect();
		std::shared_ptr<Object> greateCircle();
		std::shared_ptr<Object> greateArcCircle();
		std::shared_ptr<Object> greatePolygon();
		std::shared_ptr<Object> greatePolyLine();

	private:
		DataProvider<double>* dataprov = nullptr;
	};


};