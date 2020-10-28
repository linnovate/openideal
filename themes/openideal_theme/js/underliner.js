class Underliner {
    constructor(selector, color1, color2, thickness1, thickness2, strokeLinecap, rtl) {
        this.links = document.querySelectorAll(selector)
        this.fill = 'transparent';
        this.color1 = color1;
        this.color2 = color2;
        this.thickness1 = thickness1;
        this.thickness2 = thickness2;
        this.strokeLinecap = strokeLinecap;
        this.rtl = rtl;
        this.init();
    }

    init() {
        let self = this;

        self.links.forEach(function (link) {
            let linkWidth = parseInt(link.offsetWidth);
            let svg = self.createSVG(linkWidth);
            self.insertAfter(svg, link);
        });
    }

    setPath(pathD, color, thickness, strokeLinecap) {
        const path = document.createElementNS("http://www.w3.org/2000/svg", "path");

        path.setAttribute("d", pathD);
        path.setAttribute("fill", this.fill);
        path.setAttribute("stroke", color);
        path.setAttribute("stroke-width", thickness);
        path.setAttribute("stroke-linecap", strokeLinecap);
        path.setAttribute("stroke-dasharray", path.getTotalLength() + 10);
        path.setAttribute("stroke-dashoffset", path.getTotalLength() + 10);

        return path;
    }

    randomizePath(linkWidth) {
        let moveYMin = 5;
        let moveYMax = 12;

        let curveXMin = 15;
        let curveXMax = linkWidth; /* Width of the link */
        let curveYMin = 7;
        let curveYMax = linkWidth * 0.12; /* Making the quadratic propotional to the link width */
        //let curveYMax = 20

        let endYMin = 5;
        let endYMax = 11;

        let moveY = Math.floor(Math.random() * (moveYMax - moveYMin)) + moveYMin;
        let curveX = Math.floor(Math.random() * (curveXMax - curveXMin)) + curveXMin;
        let curveY = Math.floor(Math.random() * (curveYMax - curveYMin)) + curveYMin;
        let endY = Math.floor(Math.random() * (endYMax - endYMin)) + endYMin;

        return `M5 ${moveY} Q ${curveX} ${curveY} ${linkWidth - 7} ${endY}`
    }

    createSVG(linkWidth) {
        const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");

        svg.setAttribute("width", linkWidth);
        svg.setAttribute("height", "35");

        const path = document.createElementNS("http://www.w3.org/2000/svg", "path");
        const path2 = document.createElementNS("http://www.w3.org/2000/svg", "path");

        let pathD = this.randomizePath(linkWidth);
        let pathD2 = this.randomizePath(linkWidth);

        if(this.rtl === true) {
            pathD = this.reverseMe(pathD);
            pathD2 = this.reverseMe(pathD2);
        }

        svg.appendChild(this.setPath(pathD, this.color1, this.thickness1, this.strokeLinecap));
        svg.appendChild(this.setPath(pathD2, this.color2, this.thickness2, this.strokeLinecap));

        svg.setAttribute("focusable", false);

        return svg;
    }

    reverseMe(path) {
        /* Regex functions borrwed from 
        https://github.com/krispo/svg-path-utils/blob/master/src/svg-path-utils.js */
        let pathOperators = path.replace(/[\d,\-\s]+/g, '').split('');
        let pathNums = path.replace(/[A-Za-z,]+/g, ' ').trim().replace(/\s\s+/g, ' ').split(' ');
    
        return `${pathOperators[0]} ${pathNums[4]} ${pathNums[5]} ${pathOperators[1]} ${pathNums[2]} ${pathNums[3]} ${pathNums[0]} ${pathNums[1]}`;
    }

    // https://plainjs.com/javascript/manipulation/insert-an-element-after-or-before-another-32/
    insertAfter(el, referenceNode) {
        referenceNode.parentNode.insertBefore(el, referenceNode.nextSibling);
    }
}