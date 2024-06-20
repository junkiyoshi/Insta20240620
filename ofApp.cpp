#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->line.clear();

	float phi_deg_step = 5;
	float theta_deg_step = 5;
	auto threshold = 0.68;

	for (float radius = 100; radius <= 250; radius += 5) {

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg <= 180; theta_deg += theta_deg_step) {

				auto noise_value = ofNoise(
					radius * cos(phi_deg * DEG_TO_RAD) * 0.08,
					radius * sin(phi_deg * DEG_TO_RAD) * 0.005,
					radius * cos(theta_deg * DEG_TO_RAD) * 0.01,
					ofGetFrameNum() * 0.025);

				if (noise_value < threshold) { continue; }

				auto noise_value_1 = ofNoise(
					radius * cos(phi_deg * DEG_TO_RAD) * 0.08,
					radius * sin(phi_deg * DEG_TO_RAD) * 0.005,
					radius * cos((theta_deg - theta_deg_step) * DEG_TO_RAD) * 0.01,
					ofGetFrameNum() * 0.025);
				auto noise_value_2 = ofNoise(
					radius * cos((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.08,
					radius * sin((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.005,
					radius * cos(theta_deg * DEG_TO_RAD) * 0.01,
					ofGetFrameNum() * 0.025);
				auto noise_value_3 = ofNoise(
					radius * cos((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.08,
					radius * sin((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.005,
					radius * cos(theta_deg * DEG_TO_RAD) * 0.01,
					ofGetFrameNum() * 0.025);
				auto noise_value_4 = ofNoise(
					radius * cos(phi_deg * DEG_TO_RAD) * 0.08,
					radius * sin(phi_deg * DEG_TO_RAD) * 0.005,
					radius * cos((theta_deg + theta_deg_step) * DEG_TO_RAD) * 0.01,
					ofGetFrameNum() * 0.025);

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				if (noise_value_1 < threshold) {

					this->line.addVertex(vertices[0]); this->line.addVertex(vertices[1]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_value_2 < threshold) {

					this->line.addVertex(vertices[0]); this->line.addVertex(vertices[2]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_value_3 < threshold) {

					this->line.addVertex(vertices[1]); this->line.addVertex(vertices[3]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_value_4 < threshold) {

					this->line.addVertex(vertices[2]); this->line.addVertex(vertices[3]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
				}

				for (int i = 0; i < vertices.size(); i++) {

					ofColor face_color = ofColor(ofMap((int)radius % 25, 0, 25, 0, 255));
					ofColor line_color = ofColor(ofMap((int)radius % 25, 0, 25, 255, 0));

					this->face.addColor(face_color);
					this->line.addColor(line_color);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);
	ofRotateZ(ofGetFrameNum() * 0.77);

	this->face.draw();
	this->line.draw();

	this->cam.end();

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}