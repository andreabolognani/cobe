/* slice-pattern.js - Draw a colorful pattern on a canvas
 * Copyright (C) 2013  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

function debug(message) {

	$("#debug").append("<p>" + message + "</p>");
}

function inRange(v, min, max) {

	while (v < min) {
		v += Math.abs(max - min + 1);
	}

	while (v > max) {
		v -= Math.abs(max - min + 1);
	}

	return v;
}

function normalizeAngle(angle) {

	return inRange(angle, 0, 359);
}

function toRadians(degrees) {

	return degrees / 180 * Math.PI;
}

function drawBeachBall(args) {

	var canvas = args.canvas;
	var colors = args.colors;
	var slices = args.slices;
	var origin = args.origin;
	var radius = args.radius;
	var angleOffset = args.angleOffset;
	var start;
	var end;
	var i;

	slice = 360 / slices;

	start = normalizeAngle(angleOffset);
	end = normalizeAngle(start + slice);

	for (i = 0; i < slices; i++) {

		var color = colors[inRange(i, 0, 1)];

		canvas.drawArc({
			strokeStyle: color,
			fillStyle: color,
			closed: true,
			x: origin.x,
			y: origin.y,
			radius: radius,
			start: start,
			end: end
		});

		var x2 = origin.x + radius * Math.sin(toRadians(start));
		var y2 = origin.y - radius * Math.cos(toRadians(start));
		var x3 = origin.x + radius * Math.sin(toRadians(end));
		var y3 = origin.y - radius * Math.cos(toRadians(end));

		canvas.drawLine({
			fillStyle: color,
			closed: true,
			x1: origin.x,
			y1: origin.y,
			x2: x2,
			y2: y2,
			x3: x3,
			y3: y3
		});

		start = normalizeAngle(start + slice);
		end = normalizeAngle(end + slice);
	}
}

function drawFrame(args) {

	var slices = args.slices;
	var origin = args.origin;
	var radiusOffset = args.radiusOffset;
	var radiusIncrement = args.radiusIncrement;
	var angleOffset = args.angleOffset;

	var radius;
	var angleIncrement;

	angleIncrement = 360 / slices;

	radius = Math.max(args.canvas.width(), args.canvas.height());
	if (radius % args.radiusIncrement > 0) {
		radius = Math.floor(radius / args.radiusIncrement) * args.radiusIncrement;
	}

	while (radius > 0) {

		drawBeachBall({
			canvas: args.canvas,
			colors: args.colors,
			slices: slices,
			origin: origin,
			radius: radius,
			angleOffset: angleOffset,
		});

		// Decrease the radius to draw a smaller circle, increase
		// the angle so that colors get automatically swapped
		radius -= radiusIncrement;
		angleOffset += angleIncrement;
	}
}

function main() {

	// Configurable values
	const COLOR_LIGHT = "#F22477";
	const COLOR_DARK = "#003040";
	const SLICES = 40;
	const ANGLE_OFFSET = 90;
	const RADIUS_OFFSET = 0;
	const RADIUS_INCREMENT = 70;

	if (SLICES % 2 != 0) {
		debug("The number of slices must be even!");
		return;
	}

	var canvas = $("canvas");
	var origin = {
		x : Math.floor(canvas.width() / 2),
		y : Math.floor(canvas.height() / 2)
	};
	var colors = [
		COLOR_LIGHT,
		COLOR_DARK
	];

	drawFrame({
		canvas: canvas,
		colors: colors,
		slices: SLICES,
		origin: origin,
		angleOffset: ANGLE_OFFSET,
		radiusOffset: RADIUS_OFFSET,
		radiusIncrement: RADIUS_INCREMENT
	});
}

$(document).ready(main);
